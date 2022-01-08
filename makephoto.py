import cv2 as cv

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