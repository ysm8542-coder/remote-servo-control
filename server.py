
import asyncio
import json
import os
import websockets

clients = {
    "esp8266": None,
    "phone": set(),
}


async def handler(websocket):
    role = None
    try:
        async for raw_message in websocket:
            try:
                data = json.loads(raw_message)
            except json.JSONDecodeError:
                print(f"[WARN] JSON 파싱 실패: {raw_message}")
                continue

            msg_role = data.get("role")

            # --- 최초 등록 ---
            if msg_role == "esp8266" and role is None:
                role = "esp8266"
                clients["esp8266"] = websocket
                print("[INFO] ESP8266 연결됨")
                await websocket.send(json.dumps({"status": "registered", "role": "esp8266"}))
                continue

            if msg_role == "phone" and role is None:
                role = "phone"
                clients["phone"].add(websocket)
                print("[INFO] 핸드폰 클라이언트 연결됨")
                await websocket.send(json.dumps({"status": "registered", "role": "phone"}))
                continue

            # --- 핸드폰 -> ESP8266 : 서보 각도 명령 ---
            if role == "phone" and "angle" in data:
                esp = clients["esp8266"]
                if esp is not None:
                    await esp.send(json.dumps({"angle": data["angle"]}))
                else:
                    print("[WARN] ESP8266이 아직 연결되지 않음 - 명령 무시됨")

            # --- ESP8266 -> 모든 핸드폰 : 자이로 데이터 브로드캐스트 ---
            if role == "esp8266" and "gyro" in data:
                dead = set()
                for phone_ws in clients["phone"]:
                    try:
                        await phone_ws.send(raw_message)
                    except websockets.exceptions.ConnectionClosed:
                        dead.add(phone_ws)
                clients["phone"] -= dead

    except websockets.exceptions.ConnectionClosed:
        pass
    finally:
        if role == "esp8266":
            clients["esp8266"] = None
            print("[INFO] ESP8266 연결 종료됨")
        elif role == "phone":
            clients["phone"].discard(websocket)
            print("[INFO] 핸드폰 클라이언트 연결 종료됨")


async def main():
    port = int(os.environ.get("PORT", 8765))
    print(f"[INFO] 서버 시작 - 0.0.0.0:{port}")
    async with websockets.serve(handler, "0.0.0.0", port):
        await asyncio.Future()


if __name__ == "__main__":
    asyncio.run(main())
