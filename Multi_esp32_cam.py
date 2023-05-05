import cv2

# ESP32 URL if both cameras are connected to the same IP address use different server ports
UrlL = "http://192.168.66.179"
UrlR = "http://192.168.66.41"

# if needed, change the server port
capL = cv2.VideoCapture(UrlL + ":81/stream")
capR = cv2.VideoCapture(UrlR + ":81/stream")

if __name__ == "__main__":
    while True:
        if capL.isOpened() and capR.isOpened():
            retL, frameL = capL.read()
            retR, frameR = capR.read()

            # If frames are to big, resize them and change cv2.imshow()
            # fL = cv2.resize(frameL, (640, 480))
            # fR = cv2.resize(frameR, (640, 480))

            cv2.imshow("Left", frameL)
            cv2.imshow("Right", frameR)

            key = cv2.waitKey(1)

            # Press s to save images
            if key == ord("s"):
                cv2.imwrite("testL.jpg", frameL)
                cv2.imwrite("testR.jpg", frameR)
                print("Images saved!")

            # Press esc to exit
            elif key == 27:
                break

    cv2.destroyAllWindows()
    capL.release()
    capR.release()
