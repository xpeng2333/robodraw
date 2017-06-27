import cv2

clicked = False


def onMouse(event, x, y, flags, param):
    global clicked
    if event == cv2.EVENT_LBUTTOMUP:
        clicked = True


cameraCapture = cv2.VideoCapture(0)
cv2.namedWindow('MyWindow')
cv2.setMouseCallback('MyWindow', onMouse)

print "haha"

success, frame = cameraCapture.read()


def key():
    keycode = cv2.waitKey(1)
    if keycode != -1:
        keycode &= 0xFF
    return keycode


while success and key == -1 and not clicked:
    cv2.imshow('MyWindow', frame)
    success, frame = cameraCapture.read()

cv2.destroyWindow('MyWindow')
cameraCapture.release()
