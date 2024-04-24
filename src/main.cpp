#include "definitions.h"
using namespace BLA;
// #define TEST
void setup()
{
    Serial.begin(9600);

    digitalWrite(LED_BUILTIN, 1);

    BLA::Matrix<3> fl_dh_a = {0, 11, 13}; // in CM
    BLA::Matrix<3> fl_dh_alpha = {M_PI_2, 0, 0};
    BLA::Matrix<3> fl_dh_d = {0,-5, 0};


    testleg.setDh(fl_dh_a, fl_dh_alpha, fl_dh_d);

    testleg.attach_servos();
    //CAREFUL ITS IN RAD

    // testleg.setTheta({0.3,0,0});
    testleg.DriveLeg({0.2,-0.08,0.47});
    testleg.DriveLeg({0,0,0});

    delay(1000);

    timer0 = millis();
}

void loop()
{
    #ifdef TEST
    BLAprintMatrix(testleg.getEndEffectorPosition());
    #else
    // SECTION - constant time loop :
    if (millis() - timer0 >= LOOP_PERIODms)
    {
        timer0 = millis();

        if ((abs(init_error(1)) > min_error) || (abs(init_error(1)) > min_error) || (abs(init_error(2)) > min_error))
        {
            // init_error = testleg.JTranspIK(testtraj.get_position(0), BLAdiagonal<3>(0.1), LOOP_PERIODsec);
            init_error = testleg.JTranspIK({-4,5,-20}, BLAdiagonal<3>(0.1), LOOP_PERIODsec);
            timer2 = millis();
            // BLAprintMatrix(init_error);
            BLAprintMatrix(testleg.getEndEffectorPosition());
        }
        else if (initialization)
        {
            if (millis() - timer2 > 2e3)
            {
                Serial.println("Init_done");
                BLAprintMatrix(testleg.getTheta());
                initialization = false;
                timer1 = millis();
            }
        }

        // Matrix<3> x_d = testtraj.get_position((millis() - timer1) / 1e3);
        // Matrix<3> xd_d = testtraj.get_velocity((millis() - timer1) / 1e3);

        // if (!initialization)
        // {
        //     testleg.JInvIK(x_d, xd_d, BLAdiagonal<3>(5), LOOP_PERIODsec);
        // }

        // BLAprintMatrix(testleg.getEndEffectorPosition());
        // BLAprintMatrix(x_d);
        // BLAprintMatrix(testleg.getTheta());

        // Serial.println();
    }
    //! SECTION
    #endif
    testleg.DriveLeg();
    testleg.update_leg(testleg.getTheta()); // a bit odd. Check definition of update_leg. Generally define the  "theta" pipeline
}
