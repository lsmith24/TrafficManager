#ifndef VC2GPS_H_
#define VC2GPS_H_

#include <math.h>

#define R 6371000.0
#define I1_LAT 0.697334667
#define I1_LON -1.312344093
#define I2_LAT 0.697330304
#define I2_LON -1.312308216
#define I3_LAT 0.697356969
#define I3_LON -1.312303853
#define I4_LAT 0.697361332
#define I4_LON -1.312338275
#define ABC_B 1.728132911
#define DEF_B 0.124781345
#define GHI_B 4.87628509
#define IHG_B 1.734668199
#define JKL_B 3.307306818

enum RoadIdentifier {
    AB,
    BC,
    DE,
    EF,
    GH,
    HI,
    IH,
    HG,
    JK,
    KL
};

struct GPS_Coord {
    double lat, lon;
};

inline double inDegrees(double angleInRadians) {
    return ((angleInRadians) * 180.0 / M_PI);
}

inline GPS_Coord GetLatLon(float x, RoadIdentifier &road) {
    // select road parameters
    double start_lat, start_lon, start_bearing, dx;
    
    switch(road){
        case RoadIdentifier::AB:
            start_lat = I1_LAT;
            start_lon = I1_LON;
            start_bearing = ABC_B;
            
            dx = (x - 50)*177.84/100.0;
        break;
        case RoadIdentifier::BC:
            start_lat = I1_LAT;
            start_lon = I1_LON;
            start_bearing = ABC_B;
            
            dx = (x + 50)*177.84/100.0;
        break;    
        case RoadIdentifier::DE:
            start_lat = I2_LAT;
            start_lon = I2_LON;
            start_bearing = DEF_B;
            
            dx = (x - 50)*172.56/100.0;
        break;
        case RoadIdentifier::EF:
            start_lat = I2_LAT;
            start_lon = I2_LON;
            start_bearing = DEF_B;
            
            dx = (x + 50)*172.56/100.0;
        break;
        case RoadIdentifier::GH:
            start_lat = I3_LAT;
            start_lon = I3_LON;
            start_bearing = GHI_B;
            
            dx = (x - 50)*169.42/100.0;
        break;
        case RoadIdentifier::HI:
            start_lat = I3_LAT;
            start_lon = I3_LON;
            start_bearing = GHI_B;
            
            dx = (x + 50)*169.42/100.0;
        break;
        case RoadIdentifier::IH:
            start_lat = I4_LAT;
            start_lon = I4_LON;
            start_bearing = IHG_B;
            
            dx = (x - 50)*169.42/100.0;
        break;
        case RoadIdentifier::HG:
            start_lat = I4_LAT;
            start_lon = I4_LON;
            start_bearing = IHG_B;
            
            dx = (x + 50)*169.42/100.0;
        break;
        case RoadIdentifier::JK:
            start_lat = I4_LAT;
            start_lon = I4_LON;
            start_bearing = JKL_B;
            
            dx = (x - 50)*171.64/100.0;
        break;
        case RoadIdentifier::KL:
            start_lat = I4_LAT;
            start_lon = I4_LON;
            start_bearing = JKL_B;
            
            dx = (x + 50)*171.64/100.0;
        break;
    }

    // convert real coordinates to latitude, longitude
    double end_lat = asin( sin(start_lat)*cos(dx/R) + cos(start_lat)*sin(dx/R)*cos(start_bearing) );
    double end_lon = start_lon + atan2( sin(start_bearing)*sin(dx/R)*cos(start_lat), cos(dx/R)-sin(start_lat)*sin(end_lat) );
    
    end_lat = inDegrees(end_lat); 
    end_lon = inDegrees(end_lon);
    end_lon = fmod(end_lon + 540, 360) - 180;

    GPS_Coord coord {
        .lat = end_lat,
        .lon = end_lon
    };

    return coord;
}

#endif