#include "DisplayModulo.h"
#include "Modulo.h"

#ifdef SPARK
#include "spark_wiring.h"
#else
#include "Arduino.h"
#endif

#define swap(a, b) { int16_t t = a; a = b; b = t; }


#define FUNCTION_APPEND_OP 0
#define FUNCTION_IS_COMPLETE 1
#define FUNCTION_GET_BUTTONS 2
#define FUNCTION_RAW_WRITE 3
#define FUNCTION_IS_EMPTY 4
#define FUNCTION_GET_AVAILABLE_SPACE 5
#define FUNCTION_SET_CURRENT 6
#define FUNCTION_SET_CONTRAST 7

#define EVENT_BUTTON_CHANGED 0

static const int OpRefresh = 0;
static const int OpFillScreen = 1;
static const int OpDrawLine = 2;
static const int OpSetLineColor = 3;
static const int OpSetFillColor = 4;
static const int OpSetTextColor = 5;
static const int OpDrawRect = 6;
static const int OpDrawCircle = 7;
static const int OpDrawTriangle = 8;
static const int OpDrawString = 9;
static const int OpSetCursor = 10;
static const int OpSetTextSize = 11;
static const int OpClear = 12;

DisplayModulo::DisplayModulo() :
    BaseModulo("co.modulo.display"),
        _currentOp(-1), _opBufferLen(0), _buttonState(0),
        _isRefreshing(false), _availableSpace(0)
{
}

DisplayModulo::DisplayModulo(uint16_t deviceID) :
    BaseModulo("co.modulo.display", deviceID),
        _currentOp(-1), _opBufferLen(0), _buttonState(0),
        _isRefreshing(false), _availableSpace(0)
{
}

DisplayModulo::~DisplayModulo() {

}

void DisplayModulo::_sendOp(uint8_t *data, uint8_t len) {
    while (_availableSpace < len) {
        uint8_t receiveData[2] = {0,0};
        if (_transfer(FUNCTION_GET_AVAILABLE_SPACE, 0, 0, receiveData, 2)) {
            _availableSpace = receiveData[0] | (receiveData[1] << 8);
        } else {
            return;
        }
        

        if (_availableSpace < len) {
            delay(5);
        }
    }

    _availableSpace -= len;

    _transfer(FUNCTION_APPEND_OP, data, len, 0, 0);
}

void DisplayModulo::_beginOp(uint8_t opCode) {
    if (opCode == _currentOp) {
        return;
    }

    _currentOp = opCode;
    _opBufferLen = 1;
    _opBuffer[0] = opCode;
}

void DisplayModulo::_appendToOp(uint8_t data) {
    _opBuffer[_opBufferLen++] = data;

    if (_currentOp == OpDrawString and _opBufferLen == OP_BUFFER_SIZE-1) {
        _endOp();
    }
}

void DisplayModulo::_endOp() {
    if (_currentOp == OpDrawString) {
        _opBuffer[_opBufferLen++] = 0;
        _sendOp(_opBuffer, _opBufferLen);
        _opBufferLen = 0;
        _currentOp = -1;
    }
}


void DisplayModulo::clear() {
    _endOp();

    _waitOnRefresh();

    uint8_t sendData[] = {OpClear};
    _sendOp(sendData, 1);
}

void DisplayModulo::setLineColor(float r, float g, float b, float a) {
    r = constrain(r, 0.0, 1.0);
    g = constrain(g, 0.0, 1.0);
    b = constrain(b, 0.0, 1.0);
    a = constrain(a, 0.0, 1.0);

    _endOp();

    _waitOnRefresh();

    uint8_t sendData[] = {OpSetLineColor, 
        static_cast<uint8_t>(255*r),
        static_cast<uint8_t>(255*g),
        static_cast<uint8_t>(255*b),
        static_cast<uint8_t>(255*a)};

    _sendOp(sendData, 5);
}

void DisplayModulo::setFillColor(float r, float g, float b, float a) {
    r = constrain(r, 0.0, 1.0);
    g = constrain(g, 0.0, 1.0);
    b = constrain(b, 0.0, 1.0);
    a = constrain(a, 0.0, 1.0);

    _endOp();

    _waitOnRefresh();

    uint8_t sendData[] = {OpSetFillColor, 
        static_cast<uint8_t>(255*r),
        static_cast<uint8_t>(255*g),
        static_cast<uint8_t>(255*b),
        static_cast<uint8_t>(255*a)};
    _sendOp(sendData, 5);
}

void DisplayModulo::setTextColor(float r, float g, float b, float a) {
    r = constrain(r, 0.0, 1.0);
    g = constrain(g, 0.0, 1.0);
    b = constrain(b, 0.0, 1.0);
    a = constrain(a, 0.0, 1.0);
    _endOp();

    _waitOnRefresh();

    uint8_t sendData[] = {OpSetTextColor, 
        static_cast<uint8_t>(255*r),
        static_cast<uint8_t>(255*g),
        static_cast<uint8_t>(255*b),
        static_cast<uint8_t>(255*a)};
    _sendOp(sendData, 5);
}


void DisplayModulo::setTextSize(uint8_t size) {
    _endOp();

    _waitOnRefresh();

    uint8_t sendData[] = {OpSetTextSize, size};
    _sendOp(sendData, 2);
}

void DisplayModulo::setCursor(int x, int y)
{
    _endOp();

    _waitOnRefresh();

    uint8_t sendData[] = {OpSetCursor, static_cast<uint8_t>(x),
        static_cast<uint8_t>(y)};
    _sendOp(sendData, 3);
}

void DisplayModulo::refresh(bool flip)
{
    _endOp();

    _waitOnRefresh();

    uint8_t sendData[] = {OpRefresh, flip};
    _sendOp(sendData, 2);

    _isRefreshing = true;
}


void DisplayModulo::fillScreen(float r, float g, float b)
{
    r = constrain(r, 0.0, 1.0);
    g = constrain(g, 0.0, 1.0);
    b = constrain(b, 0.0, 1.0);

    _endOp();

    _waitOnRefresh();

    uint8_t sendData[] = {OpFillScreen,
        static_cast<uint8_t>(255*r),
        static_cast<uint8_t>(255*g),
        static_cast<uint8_t>(255*b),
        1};
    _sendOp(sendData, 5);
}


void DisplayModulo::drawLine(int x0, int y0, int x1, int y1)
{
    _endOp();

    _clipLine(&x0, &y0, &x1, &y1);

    _waitOnRefresh();

    uint8_t sendData[] = {OpDrawLine, static_cast<uint8_t>(x0),
        static_cast<uint8_t>(y0), static_cast<uint8_t>(x1),
        static_cast<uint8_t>(y1)};
    _sendOp(sendData, 5);
}

void DisplayModulo::drawRect(int x, int y, int w, int h, int radius)
{
    _endOp();


    // Entirely off screen to the right or bottom.
    if (x >= WIDTH or y >= HEIGHT) {
        return;
    }

    const int left = -64;
    const int top = -64;

    // Clip the left side to -127
    // To support rounded rects, we don't clip to 0
    if (x < left) {
        w += x-left;
        x = left;
    }

    // Clip the bottom side to -127
    // To support rounded rects, we don't clip to 0
    if (y < top) {
        h += y+top;
        y = top;
    }

    // Entirely off screen to the left or top
    if (w <= 0 or h <= 0) {
        return;
    }

    if (w > 255) {
        w = 255;
    }
    if (h > 255) {
        h = 255;
    }

    _waitOnRefresh();

    uint8_t sendData[] = {OpDrawRect, static_cast<uint8_t>(x),
        static_cast<uint8_t>(y), static_cast<uint8_t>(w),
        static_cast<uint8_t>(h), static_cast<uint8_t>(radius)};
    _sendOp(sendData, 6);
}

void DisplayModulo::drawTriangle(int x0, int y0, int x1, int y1, int x2, int y2)
{
    _endOp();

    _waitOnRefresh();

    uint8_t sendData[] = {OpDrawTriangle, 
        static_cast<uint8_t>(x0), static_cast<uint8_t>(y0),
        static_cast<uint8_t>(x1), static_cast<uint8_t>(y1),
        static_cast<uint8_t>(x2), static_cast<uint8_t>(y2)};
    _sendOp(sendData, 7);
}

void DisplayModulo::drawCircle(int x, int y, int radius)
{
    _endOp();

    _waitOnRefresh();

    uint8_t sendData[] = {OpDrawCircle, 
        static_cast<uint8_t>(x),
        static_cast<uint8_t>(y),
        static_cast<uint8_t>(radius)};
    _sendOp(sendData, 4);
}

size_t DisplayModulo::write(uint8_t c) {

    _waitOnRefresh();

    if (_currentOp != OpDrawString) {
        _endOp();
        _beginOp(OpDrawString);
    }

    _appendToOp(c);

    return 1;
}

void DisplayModulo::_rawWrite(bool dataMode, uint8_t *data, size_t len) {
    if (len > 30) {
        return;
    }

    // Before issuing a raw write we must wait until no drawing operations are
    // still in progress.
    while (!isComplete()) {
        delay(5);
    }

    uint8_t sendData[32] = {dataMode};
    for  (int i=0; i < len; i++) {
        sendData[i+1] = data[i];
    }
    _transfer(FUNCTION_RAW_WRITE, sendData, len+1, 0, 0);
}

void DisplayModulo::setCurrent(float current) {
    // Before changing contrast we must wait until no drawing operations are
    // still in progress.
    while (!isComplete()) {
        delay(5);
    }

    uint8_t sendData[] = {static_cast<uint8_t>(15*current)};
    _transfer(FUNCTION_SET_CURRENT, sendData, 1, 0, 0);
}

void DisplayModulo::setContrast(float r, float g, float b) {
    // Before changing contrast we must wait until no drawing operations are
    // still in progress.
    while (!isComplete()) {
        delay(5);
    }

    uint8_t sendData[] = {static_cast<uint8_t>(255*r),
                          static_cast<uint8_t>(255*g),
                          static_cast<uint8_t>(255*b)};
    _transfer(FUNCTION_SET_CONTRAST, sendData, 3, 0, 0);
}





bool DisplayModulo::isComplete() {
    uint8_t complete = 0;
    if (_transfer(FUNCTION_IS_COMPLETE, 0, 0, &complete, 1)) {
        return complete;
    }
    return true;
}

bool DisplayModulo::isEmpty() {
    uint8_t empty = 0;
    if (_transfer(FUNCTION_IS_EMPTY, 0, 0, &empty, 1)) {
        return empty;
    }
    return true;
}


void DisplayModulo::_waitOnRefresh()
{
    if (_isRefreshing) {
        _isRefreshing = false;
        while (!isEmpty()) {
            delay(5);
        }
    }
}

bool DisplayModulo::getButton(int button) {
    return _buttonState & (1 << button);
}

uint8_t DisplayModulo::getButtons() {
    return _buttonState;
}

void DisplayModulo::drawSplashScreen() {
    setFillColor(.27,0,.24);
    setLineColor(0,0,0);
    drawRect(0, 0, width(), height());
    setCursor(0, 40);

    setTextSize(1);
    setTextColor(1,1,1);
    print("     MODULO");

    setLineColor(0,0,0,0);
    setFillColor(1,1,1);

    drawLogo(width()/2-18, 10, 35, 26);

}

void DisplayModulo::drawLogo(int x, int y, int width, int height) {

    int lineWidth = width/7;

    drawRect(x, y, width, lineWidth);
    drawRect(x, y, lineWidth, height);
    drawRect(x+width-lineWidth, y, lineWidth, height);

    drawRect(x+lineWidth*2, y+lineWidth*2, lineWidth, height-lineWidth*2);
    drawRect(x+lineWidth*4, y+lineWidth*2, lineWidth, height-lineWidth*2);
    drawRect(x+lineWidth*2, y+height-lineWidth, lineWidth*3, lineWidth);
}

void DisplayModulo::setButtonPressCallback(EventCallback *handler) {
    _buttonPressCallback = handler;
}

void DisplayModulo::setButtonReleaseCallback(EventCallback *handler) {
    _buttonReleaseCallback = handler;
}

void DisplayModulo::_processEvent(uint8_t eventCode, uint16_t eventData) {
    if (eventCode == EVENT_BUTTON_CHANGED) {
        uint8_t buttonPressed = eventData >> 8;
        uint8_t buttonReleased = eventData & 0xFF;

        _buttonState |= buttonPressed;
        _buttonState &= ~buttonReleased;

        for (int i=0; i < 3; i++) {
            if ((buttonPressed & (1 << i)) and _buttonPressCallback) {
                _buttonPressCallback(*this, i);
            }
            if ((buttonReleased & (1 << i)) and _buttonReleaseCallback) {
                _buttonReleaseCallback(*this, i);
            }
        }
    }

}


//
// Line clipping algorithm (from wikipedia)
//

static const int INSIDE = 0; // 0000
static const int LEFT = 1;   // 0001
static const int RIGHT = 2;  // 0010
static const int BOTTOM = 4; // 0100
static const int TOP = 8;    // 1000

// Compute the bit code for a point (x, y) using the clip rectangle
// bounded diagonally by (xmin, ymin), and (xmax, ymax)

// ASSUME THAT xmax, xmin, ymax and ymin are global constants.

int DisplayModulo::_computeOutCode(double x, double y)
{
	int code = INSIDE;          // initialised as being inside of clip window

    int xmin = 0;
    int ymin = 0;
    int xmax = WIDTH-1;
    int ymax = HEIGHT-1;

	if (x < xmin)           // to the left of clip window
		code |= LEFT;
	else if (x > xmax)      // to the right of clip window
		code |= RIGHT;
	if (y < ymin)           // below the clip window
		code |= BOTTOM;
	else if (y > ymax)      // above the clip window
		code |= TOP;

	return code;
}

// Cohen–Sutherland clipping algorithm clips a line from
// P0 = (x0, y0) to P1 = (x1, y1) against a rectangle with 
// diagonal from (xmin, ymin) to (xmax, ymax).
bool DisplayModulo::_clipLine(int *xi0, int *yi0, int *xi1, int *yi1)
{
    double x0 = *xi0;
    double y0 = *yi0;
    double x1 = *xi1;
    double y1 = *yi1;

	// compute outcodes for P0, P1, and whatever point lies outside the clip rectangle
	int outcode0 = _computeOutCode(x0, y0);
	int outcode1 = _computeOutCode(x1, y1);
	bool accept = false;

    int xmin = 0;
    int ymin = 0;
    int xmax = WIDTH-1;
    int ymax = HEIGHT-1;

	while (true) {
		if (!(outcode0 | outcode1)) { // Bitwise OR is 0. Trivially accept and get out of loop
			accept = true;
			break;
		} else if (outcode0 & outcode1) { // Bitwise AND is not 0. Trivially reject and get out of loop
			break;
		} else {
			// failed both tests, so calculate the line segment to clip
			// from an outside point to an intersection with clip edge
			double x, y;

			// At least one endpoint is outside the clip rectangle; pick it.
			int outcodeOut = outcode0 ? outcode0 : outcode1;

			// Now find the intersection point;
			// use formulas y = y0 + slope * (x - x0), x = x0 + (1 / slope) * (y - y0)
			if (outcodeOut & TOP) {           // point is above the clip rectangle
				x = x0 + (x1 - x0) * (ymax - y0) / (y1 - y0);
				y = ymax;
			} else if (outcodeOut & BOTTOM) { // point is below the clip rectangle
				x = x0 + (x1 - x0) * (ymin - y0) / (y1 - y0);
				y = ymin;
			} else if (outcodeOut & RIGHT) {  // point is to the right of clip rectangle
				y = y0 + (y1 - y0) * (xmax - x0) / (x1 - x0);
				x = xmax;
			} else if (outcodeOut & LEFT) {   // point is to the left of clip rectangle
				y = y0 + (y1 - y0) * (xmin - x0) / (x1 - x0);
				x = xmin;
			}

			// Now we move outside point to intersection point to clip
			// and get ready for next pass.
			if (outcodeOut == outcode0) {
				x0 = x;
				y0 = y;
				outcode0 = _computeOutCode(x0, y0);
			} else {
				x1 = x;
				y1 = y;
				outcode1 = _computeOutCode(x1, y1);
			}
		}
	}
	if (accept) {
        *xi0 = x0;
        *yi0 = y0;
        *xi1 = x1;
        *yi1 = y1;
        return true;
	}

    return false;
}
