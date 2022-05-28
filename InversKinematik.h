#define coxaLength 0
#define fermurLength 10
#define thibiaLength 8

vec3_t rotateMatrix(vec3_t from, float deg){
    float rad = deg * PI / 180.0;
    float x = (cos(rad) * from.x) - (from.z * sin(rad));
    float z = (cos(rad) * from.z) + (from.x * sin(rad));
    from.x = x;
    from.z = z;
    return from;
}

vec3_t InversKinematik(vec3_t target){
    float thetaC = atan2(target.z, target.x) * 180/PI ;
    
    float l = sqrt(target.x * target.x + target.z * target.z);
    float thetaF1 = atan2(target.y, l) ;
    
    float a = sqrt(l * l + target.y * target.y);
    float thetaF2 = acos((pow(fermurLength, 2) + pow(a, 2) - pow(thibiaLength, 2) )/ (2 * a * fermurLength));
    float thetaF = (thetaF1 + thetaF2)* 180/PI;
    float thetaT = (acos((pow(fermurLength, 2) + pow(thibiaLength, 2) - pow(a, 2) )/ (2 * thibiaLength * fermurLength)) * 180/PI - 90);
    
    //normalisasi
    thetaC = 90 - thetaC;
    thetaT = thetaT - 90;
    return {thetaC,thetaF,thetaT};
}

ArduinoQueue<vec3_t> trajectory(vec3_t P1,vec3_t P2,vec3_t P3,vec3_t P4, float t){
    ArduinoQueue<vec3_t> result(1/t + 3);
    float mult = t;
    result.enqueue(P1);
    while(t<1){
        vec3_t Pt = pow(1-t,3) * P1 + 3*t*pow(1-t,2) * P2 + 3*t*t*(1-t) * P3 + pow(t,3) * P4;
        result.enqueue(Pt);
        t += mult;
        Serial.print(Pt.x);
        Serial.print(" ");
        Serial.print(Pt.y);
        Serial.print(" ");
        Serial.print(Pt.z);
        Serial.println(" ");
    }
    result.enqueue(P4);
    return result;
}

int mapServo(int deg){
  return map(deg, 0, 150, 512, 0);
}