#include "JNI_MCAPI.h"
#include "mcapi.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <cmath>

using namespace std;

#define DEBUG true

void _log(const char *fmt, ...) {
  #ifdef DEBUG
    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
 #endif
}

jlong _pointer(JNIEnv *env, jobject obj) {
    jclass cls = env->GetObjectClass(obj);
    jfieldID fid = env->GetFieldID(cls, "pointer", "J");
    jlong p = env->GetLongField(obj, fid);
    return p;
}

JNIEXPORT jint JNICALL Java_by_moonx_backend_mcapi_MCAPI_init
  (JNIEnv *env, jobject obj) {
   _log("init\n");

   char* pointer = (char*)malloc(sizeof(char) * mcGetApiBufferSize());
   int result = mcInit(pointer);

   if (result != 0) return result;
   jclass cls = env->GetObjectClass(obj);
   jfieldID fid = env->GetFieldID(cls, "pointer", "J");
   env->SetLongField(obj, fid, result == 0 ? (jlong)pointer:0);

   fid = env->GetFieldID(cls, "request", "Lby/moonx/backend/mcapi/Request;");
   jobject request = env->GetObjectField(obj, fid);
   cls = env->GetObjectClass(request);

   fid = env->GetFieldID(cls, "time", "D");
   jdouble time = env->GetDoubleField(request, fid);

   fid = env->GetFieldID(cls, "timeZone", "D");
   jdouble timeZone = env->GetDoubleField(request, fid);

   fid = env->GetFieldID(cls, "lon", "D");
   jdouble lon = env->GetDoubleField(request, fid);

   fid = env->GetFieldID(cls, "lat", "D");
   jdouble lat = env->GetDoubleField(request, fid);

   _log("mcSetTime: %f, %f\n", time, timeZone);

   result = mcSetTime((void*)pointer, time, timeZone);

   if (result != 0) return result;
   _log("mcSetLocation: %f, %f\n", lon, lat);
   result = mcSetLocation((void*)pointer, lon, lat);

   return result;
}

JNIEXPORT jint JNICALL Java_by_moonx_backend_mcapi_MCAPI_destroy
  (JNIEnv *env, jobject obj)  {
    _log("destroy");
    jlong pointer = _pointer(env, obj);
    return mcDestroy((void*)pointer);
}

JNIEXPORT jint JNICALL Java_by_moonx_backend_mcapi_MCAPI_eclipticPosition
  (JNIEnv *env, jobject obj) {
  _log("mcGetEclipticPosition\n");

  double lon;
  double lat;
  double distance;
  jlong pointer = _pointer(env, obj);
  int result = mcGetEclipticPosition((void*)pointer, lon, lat, distance);
  if (result == 0) {
        _log("EclipticPosition: %f,%f,%f\n", lon,lat, distance);
        jclass c = env->FindClass("by/moonx/backend/mcapi/EclipticPosition");
        jmethodID m = env->GetMethodID(c, "<init>", "(DDD)V");
        jobject position = env->NewObject(c,m, lon, lat, distance);

        jclass cls = env->GetObjectClass(obj);
        jfieldID fid = env->GetFieldID(cls, "eclipticPosition", "Lby/moonx/backend/mcapi/EclipticPosition;");
        env->SetObjectField(obj, fid, position);
  }
  return result;
}

JNIEXPORT jint JNICALL Java_by_moonx_backend_mcapi_MCAPI_newMoon
  (JNIEnv *env, jobject obj) {
    jlong pointer = _pointer(env, obj);
    double when;
    int result = mcGetNewMoon((void*)pointer, when);

    if (result == 0) {
        jclass cls = env->GetObjectClass(obj);
        jfieldID fid = env->GetFieldID(cls, "newMoon", "D");
        env->SetDoubleField(obj, fid, when);
    }
    return result;
}


JNIEXPORT jint JNICALL Java_by_moonx_backend_mcapi_MCAPI_enumeratePhases
  (JNIEnv *env, jobject obj, jdouble start, jdouble end) {
  _log("enumeratePhases: %f, %f\n", start, end);
  jlong pointer = _pointer(env, obj);
  return mcEnumeratePhases((void*)pointer, start, end);
}

JNIEXPORT jint JNICALL Java_by_moonx_backend_mcapi_MCAPI_nextPhase
  (JNIEnv *env, jobject obj) {
    _log("nextPhase\n");
    jlong pointer = _pointer(env, obj);

    double when;
    int phase;

    int result = mcNextPhase((void*)pointer, when, phase);
    if (result == 0) {
        _log("nextPhase: %f,%d\n", when, phase);
        jclass c = env->FindClass("by/moonx/backend/mcapi/Phase");
        jmethodID m = env->GetMethodID(c, "<init>", "(DI)V");
        jobject nextPhase = env->NewObject(c,m, when, phase);

        jclass cls = env->GetObjectClass(obj);
        jfieldID fid = env->GetFieldID(cls, "nextPhase", "Lby/moonx/backend/mcapi/Phase;");
        env->SetObjectField(obj, fid, nextPhase);
    }
    return result;
}

JNIEXPORT jint JNICALL Java_by_moonx_backend_mcapi_MCAPI_riseCount
  (JNIEnv *env, jobject obj) {
    _log("riseCount\n");
    jlong pointer = _pointer(env, obj);

    int count;
    int result = mcGetRiseCount((void*)pointer, count);

    if (result == 0) {
        jclass cls = env->GetObjectClass(obj);
        jfieldID fid = env->GetFieldID(cls, "riseCount", "I");
        env->SetIntField(obj, fid, count);
    }
    return result;
}


JNIEXPORT jint JNICALL Java_by_moonx_backend_mcapi_MCAPI_phaseInformation
  (JNIEnv *env, jobject obj) {
  _log("mcGetPhaseInformation\n");

  double age;
  double count;
  double xz;
  jlong pointer = _pointer(env, obj);
  int result = mcGetPhaseInformation((void*)pointer, age, count, xz);
  if (result == 0) {
        _log("PhaseInformation: %f,%f,%f\n", age, count, xz);
        jclass c = env->FindClass("by/moonx/backend/mcapi/PhaseInformation");
        jmethodID m = env->GetMethodID(c, "<init>", "(DDD)V");
        jobject position = env->NewObject(c, m, age, count, xz);

        jclass cls = env->GetObjectClass(obj);
        jfieldID fid = env->GetFieldID(cls, "phaseInformation", "Lby/moonx/backend/mcapi/PhaseInformation;");
        env->SetObjectField(obj, fid, position);
  }
  return result;
}


