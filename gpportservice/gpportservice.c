#include <jni.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <poll.h>
#include <stdlib.h>

#define DRAWER_ADDRESS		(0x482)
#define MMIO_FILE	"/dev/port"

static unsigned char portread() {
	unsigned char cd_status;
	int fd = open(MMIO_FILE, O_RDONLY);
	if (fd < 0) {
		perror("open /dev/port (read)");
		return 0xFF;
	}

	ssize_t rd = pread(fd, &cd_status, 1, DRAWER_ADDRESS);

	close(fd);

	if (rd < 0) {
		perror("pread /dev/port");
		return 0xFF;
	}
	poll(NULL, 0, 120);

	return cd_status;
}

static void portwrite(unsigned char cd_status) {

	int fd = open(MMIO_FILE, O_WRONLY);
	if (fd < 0) {
		perror("open /dev/port (write)");
		return;
	}

	ssize_t written = pwrite(fd, &cd_status, 1, DRAWER_ADDRESS);
	close(fd);

	if (written < 0) {
		perror("pwrite /dev/port");
	}
	poll(NULL, 0, 120);	

	return;
}

JNIEXPORT jint JNICALL
Java_custom_hardware_gpportservice_gpportserviceJNI_opendrawer(JNIEnv *env __unused, jclass clazz, jint drawer_number) {

	unsigned char cd_status;
	
	cd_status = portread();
	cd_status |= (drawer_number == 1 ? 0x10 : 0x80);
	portwrite(cd_status);

	cd_status = portread();
	cd_status &= ~(drawer_number == 1 ? 0x10 : 0x80);
	portwrite(cd_status);

	return 0;
}

JNIEXPORT jbyte JNICALL
Java_custom_hardware_gpportservice_gpportserviceJNI_getstatus(JNIEnv *env __unused, jclass clazz) {
	return portread();
}

static void throwIllegalArgument(JNIEnv *env, const char *msg) {
	jclass exClass = (*env)->FindClass(env, "java/lang/IllegalArgumentException");
	if (exClass != NULL) {
		(*env)->ThrowNew(env, exClass, msg);
	}
}

JNIEXPORT jint JNICALL
Java_custom_hardware_gpportservice_gpportserviceJNI_portWrite(JNIEnv *env __unused, jclass clazz,
                                                jstring path, jbyteArray data,
                                                jint size, jint addr) {
	if (size < 0) {
		throwIllegalArgument(env, "Negative size not allowed");
		return -EINVAL;
	}

	const char *dev = (*env)->GetStringUTFChars(env, path, 0);
	if (dev == NULL) return -ENOMEM;

	int fd = open(dev, O_WRONLY);
	if (fd < 0) {
		int err = -errno;
		(*env)->ReleaseStringUTFChars(env, path, dev);
		return err;
	}

	jsize len = (*env)->GetArrayLength(env, data);
	if (size > len) size = len;

	jbyte *buf = (*env)->GetByteArrayElements(env, data, NULL);
	if (buf == NULL) {
		close(fd);
		(*env)->ReleaseStringUTFChars(env, path, dev);
		return -ENOMEM;
	}
	poll(NULL, 0, 120);
	ssize_t written = pwrite(fd, buf, size, (off_t)addr);

	(*env)->ReleaseByteArrayElements(env, data, buf, JNI_ABORT);
	close(fd);
	(*env)->ReleaseStringUTFChars(env, path, dev);

	if (written < 0) {
		return -errno;
	}

	return (jint)written;
}

JNIEXPORT jint JNICALL
Java_custom_hardware_gpportservice_gpportserviceJNI_portRead(JNIEnv *env __unused, jclass clazz,
                                               jstring path, jbyteArray buffer,
                                               jint size, jint addr) {
	if (size < 0) {
		throwIllegalArgument(env, "Negative size not allowed");
		return -EINVAL;
	}

	const char *dev = (*env)->GetStringUTFChars(env, path, 0);
	if (dev == NULL) return -ENOMEM;

	int fd = open(dev, O_RDONLY);
	if (fd < 0) {
		int err = -errno;
		(*env)->ReleaseStringUTFChars(env, path, dev);
		return err;
	}

	jsize len = (*env)->GetArrayLength(env, buffer);
	if (size > len) size = len;

	jbyte *buf = (*env)->GetByteArrayElements(env, buffer, NULL);
	if (buf == NULL) {
		close(fd);
		(*env)->ReleaseStringUTFChars(env, path, dev);
		return -ENOMEM;
	}
	poll(NULL, 0, 120);
	ssize_t rd = pread(fd, buf, size, (off_t)addr);

	if (rd >= 0 && rd < size) {
		memset(buf + rd, 0, size - rd);
	}

	(*env)->ReleaseByteArrayElements(env, buffer, buf, (rd >= 0 ? 0 : JNI_ABORT));
	close(fd);
	(*env)->ReleaseStringUTFChars(env, path, dev);

	if (rd < 0) {
		return -errno;
	}

	return (jint)rd;
}
