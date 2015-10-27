#include "Main.hpp"

using namespace std;

State s(State::NOP);

void ShowHelp(char* arg){
    cout << "Configuration conversion utility.\nUsage:\n\t" << arg << " <infile> <outfile>" << endl;
}

uint16_t EndianCvt(uint16_t val){
    uint8_t v1 = val&0xFF;
    uint8_t v2 = val >> 8;
    return v2 | (v1 << 8);
}

void WriteReg(ofstream& out, uint16_t addr, uint16_t val){
    addr = EndianCvt(addr);
    if(s != State::REG_WR){
        char o = 0xF1;
        out.write(&o, 1);
        s = State::REG_WR;
    }
    out.write((char*)&addr, 2);
    out.write((char*)&val, 2);
}

void TestReg(ofstream& out, uint16_t addr, uint16_t val, uint16_t mask){
    addr = EndianCvt(addr);
    if(s != State::REG_RD){
        char o = 0xF3;
        out.write(&o, 1);
        s = State::REG_RD;
    }
    out.write((char*)&addr, 2);
    out.write((char*)&val, 2);
    out.write((char*)&mask, 2);
}

void WriteVar(ofstream& out, uint16_t addr, uint16_t val){
    addr = EndianCvt(addr);
    if(s != State::VAR_WR){
        char o = 0xF2;
        out.write(&o, 1);
        s = State::VAR_WR;
    }
    out.write((char*)&addr, 2);
    out.write((char*)&val, 2);
}

void TestVar(ofstream& out, uint16_t addr, uint16_t val, uint16_t mask){
    addr = EndianCvt(addr);
    if(s != State::VAR_RD){
        char o = 0xF4;
        out.write(&o, 1);
        s = State::VAR_RD;
    }
    out.write((char*)&addr, 2);
    out.write((char*)&val, 2);
    out.write((char*)&mask, 2);
}

void Delay(ofstream& out, uint8_t val){
    char o = 0xFD;
    out.write(&o, 1);
    out.write((char*)&val, 1);
}

void End(ofstream& out){
    char o = 0xFF;
    out.write(&o, 1);
}

int main(int argc, char** argv){
    if(argc < 3){
        ShowHelp(argv[0]);
        return 0;
    }

    ifstream in(argv[1]);
    ofstream out(argv[2], std::ios::binary);
    if(!in.is_open() || !out.is_open()) throw runtime_error("Can't open in/outfile");

    char d;

    in >> skipws >> hex;

    while(!in.eof()){
        in >> d;
        if(d == ';') in.ignore(numeric_limits<streamsize>::max(), '\n');
        else if(d == 'a'){
            uint16_t addr;
            char d2;
            in >> addr;
            in >> d2;
            if(d2 == 'w'){
                uint16_t val;
                in >> val;
                WriteReg(out, addr, val);
            }
            else if(d2 == 'r'){
                uint16_t val, mask;
                char d3;
                in >> val;
                in >> d3;
                if(d3 == 'm') in >> mask;
                else mask = 0xFFFF;
                TestReg(out, addr, val, mask);
            } else throw runtime_error("Bad action symbol in register operation");
        }
        else if(d == 'v'){
            uint16_t addr;
            char d2;
            in >> addr;
            in >> d2;
            if(d2 == 'W'){
                uint16_t val;
                in >> val;
                WriteVar(out, addr, val);
            }
            else if(d2 == 'R'){
                uint16_t val, mask;
                char d3;
                in >> val;
                in >> d3;
                if(d3 == 'm') in >> mask;
                else mask = 0xFFFF;
                TestVar(out, addr, val, mask);
            } else throw runtime_error("Bad action symbol in variable operation");
        }
        else if(d == 'D'){
            uint16_t val;
            in >> val;
            Delay(out, val);
        }
        else if(d == '|') break;
        else throw runtime_error("Bad operation symbol");
    }
    End(out);

    in.close();
    out.close();

    return 0;
}
