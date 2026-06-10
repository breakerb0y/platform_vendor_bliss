/*
 * COPYRIGHT (C) 2005-2026 SICOM Systems Inc
 * 1684 S. Broad Street Suite 300
 * Lansdale, PA 19446
 * 215-489-2500, FAX: 215-489-2769
 */

#include <jni.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <detectposhw.h>
#include "version.h"

int main(int argc, char **argv) {
	if (argc == 2 && (!strcmp(argv[1], "--version") || !strcmp(argv[1], "-v"))) {
		printf("Sicom DETECTPOSHW version " VERSION " (library version %s)\n", detectposhw_get_version());
		return 0;
	}

	printf("%s\n", detectposhw_get_hw_name(detectposhw()));	
	return 0;
}

JNIEXPORT jint JNICALL
Java_src_runDetect(JNIEnv *env, jobject thiz, jobjectArray args) {
    int argc = (*env)->GetArrayLength(env, args);

    char *argv[argc];

    for(int i=0;i<argc;i++)
    {
        jstring str = (jstring)(*env)->GetObjectArrayElement(env, args, i);
        argv[i] = (char*)(*env)->GetStringUTFChars(env, str, 0);
    }

    return main(argc, argv);
}
