#include <iostream>
#include <graphics.h>
#define White 15

using namespace std;

int gd = 5, gm;
int pos_x = 50, pos_y = 100;

string getSignal() {
    string signal;
    cout << "Enter signal to be encoded" << endl;
    cin >> signal;
    return signal;
}

int getEncoding() {
    int choice;
    cout
            << "Choose a type of Encoding\n1) Unipolar NRZ\n2) Polar NRZ\n3) NRZ Inverted\n4) RZ\n5) Manchester\n6) Differential Manchester"
            << endl;
    cin >> choice;
    return choice;
}

void signalMove(int offset_x, int offset_y) {
    line(pos_x, pos_y, pos_x + offset_x, pos_y + offset_y);
    pos_x = pos_x + offset_x;
    pos_y = pos_y + offset_y;
}

void signalShift(int new_x, int new_y) {
    line(pos_x, pos_y, new_x, new_y);
    pos_x = new_x;
    pos_y = new_y;
}

void drawAxes() {
    line(0, pos_y, 640, pos_y);
    line(pos_x, 0, pos_x, 200);
}

void initGraph() {
    initgraph(&gd, &gm, nullptr);
    drawAxes();
    setcolor(WHITE);
}

class Encoding {
protected:
    string signal;
    int logic_high = -20;
    int logic_low = 20;
    int base = 0;
    int distance = 20;
public:
    explicit Encoding(string sgn) {
        signal = sgn;
    }
};

class Unipolar_NRZ : Encoding {
private:
    int logic_low = 0;
public:
    explicit Unipolar_NRZ(string sgn) : Encoding(sgn) {};
    void draw() {
        for (char c: signal) {
            if (c == '1')
                one();
            else
                zero();
        }
    }
    void zero() {
        signalShift(pos_x, 100 + logic_low);
        signalMove(distance, 0);
    }
    void one() {
        signalShift(pos_x, 100 + logic_high);
        signalMove(distance, 0);
    }
};

class Polar_NRZ : Encoding {
public:
    explicit Polar_NRZ(string sgn) : Encoding(sgn) {};
    void draw() {
        for (char c: signal) {
            if (c == '1')
                one();
            else
                zero();
        }
    }
    void zero() {
        signalShift(pos_x, 100 + logic_high);
        signalMove(distance, 0);
    }
    void one() {
        signalShift(pos_x, 100 + logic_low);
        signalMove(distance, 0);
    }
};

class NRZ_I : Encoding {
public:
    explicit NRZ_I(string sgn) : Encoding(sgn) {};
    void draw() {
        pos_y = 100 + logic_high;
        for (char c: signal) {
            if (c == '1')
                one();
            else
                zero();
        }
    }
    void zero() {
        signalMove(distance, 0);
    }
    void one() {
        if (pos_y == 100 + logic_low)
            signalShift(pos_x, 100 + logic_high);
        else if (pos_y == 100 + logic_high)
            signalShift(pos_x, 100 + logic_low);
        signalMove(distance, 0);
    }
};

class RZ : Encoding {
public:
    explicit RZ(string sgn) : Encoding(sgn) {};

    void draw() {
        for (char c: signal) {
            if (c == '1')
                one();
            else
                zero();
        }
    }
    void zero() {
        signalShift(pos_x, 100 + logic_low);
        signalMove(distance, 0);
        signalShift(pos_x, 100 + base);
        signalMove(distance, 0);
    }
    void one() {
        signalShift(pos_x, 100 + logic_high);
        signalMove(distance, 0);
        signalShift(pos_x, 100 + base);
        signalMove(distance, 0);
    }
};

class Manchester : Encoding {
public:
    explicit Manchester(string sgn) : Encoding(sgn) {};
    void draw() {
        for (char c: signal) {
            if (c == '1')
                one();
            else
                zero();
        }
    }
    void zero() {
        signalShift(pos_x, 100 + logic_high);
        signalMove(distance, 0);
        signalShift(pos_x, 100 + logic_low);
        signalMove(distance, 0);
    }
    void one() {
        signalShift(pos_x, 100 + logic_low);
        signalMove(distance, 0);
        signalShift(pos_x, 100 + logic_high);
        signalMove(distance, 0);
    }
};

class Diff_Manchester : Encoding {
    int prev = 2;
public:
    explicit Diff_Manchester(string sgn) : Encoding(sgn) {};
    void draw() {
        for (char c: signal) {
            if (c == '1') {
                int n = (prev == 2 ? 1 : 2);
                pattern(n);
                prev = n;
            } else {
                pattern(prev);
            }
        }
    }
    void pattern(int num) {
        if (num == 2) {
            signalShift(pos_x, 100 + logic_low);
            signalMove(distance, 0);
            signalShift(pos_x, 100 + logic_high);
            signalMove(distance, 0);
        } else if (num == 1) {
            signalShift(pos_x, 100 + logic_high);
            signalMove(distance, 0);
            signalShift(pos_x, 100 + logic_low);
            signalMove(distance, 0);
        }
    }
};
int main() {
    string sgn;
    int c;
    sgn = getSignal();
    c = getEncoding();
    initGraph();
        switch (c) {
        case 1: {
            Unipolar_NRZ signal(sgn);
            signal.draw();
            break;
        }
        case 2: {
            Polar_NRZ signal(sgn);
            signal.draw();
            break;
        }
        case 3: {
            NRZ_I signal(sgn);
            signal.draw();
            break;
        }
        case 4: {
            RZ signal(sgn);
            signal.draw();
            break;
        }
        case 5: {
            Manchester signal(sgn);
            signal.draw();
            break;
        }
        case 6: {
            Diff_Manchester signal(sgn);
            signal.draw();
            break;
        }
    }
    getch();
    closegraph();
}
