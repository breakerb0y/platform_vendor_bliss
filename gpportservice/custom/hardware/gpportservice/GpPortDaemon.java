package custom.hardware.gpportservice;

import android.os.Looper;
import android.os.ServiceManager;
import android.util.Log;

public final class GpPortDaemon {
	private static final String TAG = "GpPortDaemon";
	private static final String SERVICE_NAME = "gpportservice";

	private static final IGpPortService.Stub sBinder = new IGpPortService.Stub() {
		@Override public int opendrawer(int drawer_number) { return gpportserviceJNI.opendrawer(drawer_number); }
		@Override public byte getstatus() { return gpportserviceJNI.getstatus(); }
		@Override public int write(String path, byte[] data, int size, int addr) { return gpportserviceJNI.portWrite(path, data, size, addr); }
		@Override public int read(String path, byte[] buffer, int size, int addr) { return gpportserviceJNI.portRead(path, buffer, size, addr); }
	};

	public static void main(String[] args) {
		Log.i(TAG, "main: starting gpport daemon");

		if (!gpportserviceJNI.isLibraryLoaded()) {
			try {
				System.loadLibrary("gpportservice");
				Log.i(TAG, "main: loaded native library gpportservice");
			} catch (Throwable t) {
			Log.e(TAG, "main: failed to load native library gpportservice", t);
			}
		} else {
			Log.i(TAG, "main: native library already loaded");
		}

		try {
			ServiceManager.addService(SERVICE_NAME, sBinder);
			Log.i(TAG, "main: ServiceManager.addService successful for " + SERVICE_NAME);
		} catch (Throwable t) {
			Log.e(TAG, "main: failed to add service to ServiceManager", t);
		}

		Looper.prepare();
		Log.i(TAG, "main: entering Looper.loop()");
		Looper.loop();
	}
}

