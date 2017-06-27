import numpy
import cv2
import requests
import sys
HEADERS = {
    'User-Agent': 'Mozilla/5.0 (Macintosh;Inetl Mac OS X 10.9;' + 'rv:25.0) GECKO/20100101 Firefox/25.0'
}


def validdateResponse(response):
    statusCode = response.status_code
    if statusCode == 200:
        return True
    url = response.request.url
    print >>sys.stderr, 'Received unexpected status code (%d) when requesting %s' % (
        statusCode, url)
    return False


def cvImageFromUrl(url):
    response = requests.get(url, headers=HEADERS)
    if not validdateResponse(response):
        return None
    imageData = numpy.fromstring(response.content, numpy.uint8)
    image = cv2.imdecode(imageData, cv2.IMREAD_COLOR)
    if image is None:
        print >>sys.stderr, 'Failed to decode image from content of %s' % url
    return image


def main():
    image = cvImageFromUrl('http://nummist.com/images/ceiling.gaze.jpg')
    if image is not None:
        cv2.imwrite('image.png', image)


if __name__ == '__main__':
    main()
