import requests
import cv2 as cv
import eel
import serial
import json
import time


@eel.expose()
def make_photo():
    key = 1
    cap = cv.VideoCapture(1)
    cap.set(cv.CAP_PROP_FRAME_WIDTH, 320)
    cap.set(cv.CAP_PROP_FRAME_HEIGHT, 240)

    while True:
        k = cv.waitKey(1)
        value, frame = cap.read()
        cv.imshow("Bag", frame)

        if k == ord('p'):
            if key <= 5:
                print(f"photo{key}")
                cv.imwrite(f'photo/bag{key}.png', frame)
                key += 1
            else:
                break

    cap.release()
    cv.destroyAllWindows()
    key = 1
    cap = cv.VideoCapture(2)
    cap.set(cv.CAP_PROP_FRAME_WIDTH, 320)
    cap.set(cv.CAP_PROP_FRAME_HEIGHT, 240)

    while True:
        k = cv.waitKey(1)
        value, frame = cap.read()
        cv.imshow("Face", frame)

        if k == ord('p'):
            cv.imwrite(f'photo/Face.png', frame)
            key += 1
            break

    cap.release()
    cv.destroyAllWindows()


@eel.expose()
def send(name, surname, flight, pat, weight):
    print(name)
    print(surname)
    print(flight)

    files = {
        'pic1': ("bag1.png", open('./photo/bag1.png', 'rb'), 'image/png'),
        'pic2': ("bag2.png", open('./photo/bag2.png', 'rb'), 'image/png'),
        'pic3': ("bag3.png", open('./photo/bag3.png', 'rb'), 'image/png'),
        'pic4': ("bag4.png", open('./photo/bag4.png', 'rb'), 'image/png'),
        'face': ("face.png", open('./photo/face.png', 'rb'), 'image/png')
    }

    city = json.load(open("settings.json", "r"))

    data = {
        "name": name,
        "surname": surname,
        "flight": flight,
        "patronymics": pat,
        "weight": weight,
        "city": city["city"]
    }

    code = requests.post(
        'https://find-suitcase.herokuapp.com/upload', files=files, data=data)
    id = code.text
    print(id)
    ser = serial.Serial("com7", 9600, timeout=5)
    time.sleep(2)
    ser.setDTR(0)
    time.sleep(2)
    ser.write(id.encode())
    statuscode = ser.readline()
    print(str(statuscode)[:-5][-3:])
    eel.statuscode(f"Status Code {str(statuscode)[:-5][-3:]}")
    ser.close()


eel.init("web")
eel.start("index.html", size=(500, 600))
