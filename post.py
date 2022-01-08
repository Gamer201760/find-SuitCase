import requests
import cv2 as cv
import eel

@eel.expose()
def make_photo():
    key = 1 
    cap = cv.VideoCapture(0)
    cap.set(cv.CAP_PROP_FRAME_WIDTH, 320)
    cap.set(cv.CAP_PROP_FRAME_HEIGHT, 240)


    while True:
        k= cv.waitKey(0)
        value, frame = cap.read()
        cv.imshow("Bafg", frame)

        if k == ord('p'):
            if key <= 4:
                print(f"photo{key}")
                cv.imwrite(f'photo/bag{key}.png', frame)
                key+=1
            else:
                break
    cap.release()
    cv.destroyAllWindows()

@eel.expose()
def send(name, fname, flight):
    print(name)
    print(fname)
    print(flight)

    files = {
    'pic1': ("bag1.png",open('./photo/bag1.png', 'rb'), 'image/png'),
    'pic2': ("bag2.png",open('./photo/bag2.png', 'rb'), 'image/png'),
    'pic3': ("bag3.png",open('./photo/bag3.png', 'rb'), 'image/png'),
    'pic4': ("bag4.png",open('./photo/bag4.png', 'rb'), 'image/png')
}

    data = {
    "name": name,
    "f_name": fname,
    "flight": flight
    }
    
    code=requests.post('http://192.168.0.101/upload', files=files, data=data)   
    print(code)

eel.init("web")
eel.start("index.html", size=(500,500))