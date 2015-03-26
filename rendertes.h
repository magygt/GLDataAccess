//general thoughts
#include <string.h>
#include <vector>
using namespace std;
#define LIGHTNUM 8
#define LIGHTPROPERTY 10

unsigned int MATER[] = {0x1200, 0x1201, 0x1202, 0x1600, 0x1601};
//AMBIENT, DIFFUSE, SPECULAR, EMISSION, SPECULAR


class Render{
private:
    int status[8];//0:light 1:object 2:view 3:fog....
    bool syncFrom, syncTo;//When the local data have changed by itself, syncTo turn to 0
    //when finished copy data from status 0 to 7, syncFrom = 1
    //when syncTo && syncFrom , glFlush or glSwapBuffers
    int state;//state = 0, state |= (bool)status[i] << i; 
    //light
    float light[LIGHTNUM][LIGHTPROPERTY][4];//light[i][j] where i is status[0] >> i & 1
    //i also represent 0x4000 + i which is
    /*#define GL_LIGHT0                         0x4000
     *#define GL_LIGHT1                         0x4001
     *#define GL_LIGHT2                         0x4002
     *#define GL_LIGHT3                         0x4003
     *#define GL_LIGHT4                         0x4004
     *#define GL_LIGHT5                         0x4005
     *#define GL_LIGHT6                         0x4006
     *#define GL_LIGHT7                         0x4007
     */

    //j = 0x1209 - 0x1200 which is
    /*#define GL_AMBIENT                        0x1200
     *#define GL_DIFFUSE                        0x1201
     *#define GL_SPECULAR                       0x1202
     *#define GL_POSITION                       0x1203
     *#define GL_SPOT_DIRECTION                 0x1204
     *#define GL_SPOT_EXPONENT                  0x1205
     *#define GL_SPOT_CUTOFF                    0x1206
     *#define GL_CONSTANT_ATTENUATION           0x1207
     *#define GL_LINEAR_ATTENUATION             0x1208
     *#define GL_QUADRATIC_ATTENUATION          0x1209
     */ 

    //object
    int object[5];//can be replaced by bit in status[1]  
    //0 begin type 0x0000 + object[0]
    //1 specific property of begin type
    //2 enable array 0x8074 + object[2] >> (0-5) & 1
    //3 length of array
    //4 the order of translate and rotate
     
    float shift[3][4];
    //0 scale
    //1 translate
    //2 rotate
    
    //...
    double view[5][3];
    //0 eye
    //1 dir
    //2 head
    //3 4 box vertex

    float material[5][4];
    //

    float fog[3][4];
    //0 fog color
    //1 density, start, end
    //formula (optional)


    vector<int> objlist;
public:
    Render():state(0),syncTo(1),syncFrom(0){};
    Render(int s[])
    {
        syncTo = 1;
        syncFrom = 0;
        memcpy(status, s, 8*sizeof(int));
        setState();
    }
    void setState()
    {
        state = 0;
        for (int i = 0; i != 8; ++i)
            state |= (bool)status[i] << i;
    }
    
    void setLight(int n, float *data)
    {
        memcpy(&light[n][0][0], data, sizeof(float)*4*10);
    } 

    void setObject(int data[])
    {
        memcpy(object, data, sizeof(int)*5);
    }

    void setObjectShift(float *data)
    {
        memcpy(&shift[0][0], data, sizeof(float)*3*4);
    }

    void setView(double *data)
    {
        memcpy(&view[0][0], data, sizeof(double)*5*3); 
    } 

    void setMaterial(float *data)
    {
        memcpy(&material[0][0], data, sizeof(float)*5*4);
    }

    void setFog(float *data)
    {
        memcpy(&fog[0][0], data, sizeof(float)*3*4);
    }

    void setObjList(vector<int> &vi)
    {
        objlist = vi;
    }

    void setObjectArray(int i, int size, double array[]);

    void setNormArray(int i, int size, double array[]);

    void unArchive(int w, int h);


    void loadLight();//can be replaced by array of func ptr like void *fp[8]()
    //and loadLight equals to fp[0]()

    void loadObject();

    void loadView();
    //projection and Ortho have different defination on box in view array

    void loadMaterial();

    void loadFog();

    void loadAll()
    {
        //if(syncFrom)
        //{
            //loadLight();
            //loadView();
            //loadObject();
        //...
        //} 
        //if(!syncTo)
            //sync();
        //glFlush();

    }
};

