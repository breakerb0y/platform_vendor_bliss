package custom.hardware.gpportservice;

import android.util.Log;

public class gpportserviceJNI {
	private static final String TAG = "gpportserviceJNI";
	private static final String LIB_NAME = "gpportservice";
	private static boolean sLibraryLoaded = false;

	static {
		try {
			System.loadLibrary(LIB_NAME);
			sLibraryLoaded = true;
			Log.i(TAG, "Successfully loaded native library: " + LIB_NAME);
		} catch (UnsatisfiedLinkError e) {
			Log.e(TAG, "Failed to load native library: " + LIB_NAME, e);
		} catch (Throwable t) {
			Log.e(TAG, "Unexpected error loading native library: " + LIB_NAME, t);
		}
	}

	public static native int opendrawer(int drawer_number);
	public static native byte getstatus();
	public static native int portWrite(String path, byte[] data, int size, int addr);
	public static native int portRead(String path, byte[] buffer, int size, int addr);

	public static boolean isLibraryLoaded() {
		return sLibraryLoaded;
	}
}
