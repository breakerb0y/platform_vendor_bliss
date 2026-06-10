package custom.hardware.gpportservice;

import android.app.Service;
import android.content.Intent;
import android.os.IBinder;
import android.os.RemoteException;
import android.os.ServiceManager;
import android.util.Log;

import custom.hardware.gpportservice.IGpPortService;

public class GpPortService extends Service {
	private static final String TAG = "GpPortService";
	private volatile IGpPortService mRemoteSvc;
	private volatile IBinder mRemoteBinder;

	private final IGpPortService.Stub mBinder = new IGpPortService.Stub() {

		@Override
		public int opendrawer(int drawer_number) {
			Log.i(TAG, "opendrawer " + drawer_number);

			IGpPortService svc = getRemoteService();
			if (svc == null) {
				Log.e(TAG, "getRemoteService returned null");
				return -1;
			}
			try {
				return svc.opendrawer(drawer_number);
			} catch (RemoteException e) {
				Log.e(TAG, "opendrawer remote failed", e);
				return -1;
			}
		}

		@Override
		public byte getstatus() {
			Log.i(TAG, "getstatus");

			IGpPortService svc = getRemoteService();
			if (svc == null) {
				Log.e(TAG, "getRemoteService returned null");
				return (byte)0;
			}
			try {
				return svc.getstatus();
			} catch (RemoteException e) {
				Log.e(TAG, "getstatus remote failed", e);
				return (byte)0;
			}
		}

		@Override
		public int write(String path, byte[] data, int size, int addr) {
			Log.i(TAG, "write " + path + " size=" + size + " addr=" + addr);

			IGpPortService svc = getRemoteService();
			if (svc == null) {
				Log.e(TAG, "getRemoteService returned null");
				return -1;
			}
			try {
				return svc.write(path, data, size, addr);
			} catch (RemoteException e) {
				Log.e(TAG, "write remote failed", e);
				return -1;
			}
		}

		@Override
		public int read(String path, byte[] buffer, int size, int addr) {
			Log.i(TAG, "read " + path + " size=" + size + " addr=" + addr);

			IGpPortService svc = getRemoteService();
			if (svc == null) {
				Log.e(TAG, "getRemoteService returned null");
				return -1;
			}
			try {
				return svc.read(path, buffer, size, addr);
			} catch (RemoteException e) {
				Log.e(TAG, "read remote failed", e);
				return -1;
			}
		}
	};

	@Override
	public IBinder onBind(Intent intent) {
		return mBinder;
	}

	private synchronized IGpPortService getRemoteService() {
		if (mRemoteSvc != null) return mRemoteSvc;

		IBinder binder = ServiceManager.getService("gpportservice");
		if (binder == null) {
			Log.w(TAG, "ServiceManager.getService returned null");
			return null;
		}

		IGpPortService svc = IGpPortService.Stub.asInterface(binder);
		if (svc == null) {
			Log.w(TAG, "Stub.asInterface returned null");
			return null;
		}

		final IBinder remoteBinder = binder;
		try {
			remoteBinder.linkToDeath(new IBinder.DeathRecipient() {
                		@Override
				public void binderDied() {
					Log.w(TAG, "remote binder died, clearing cache");
					synchronized (GpPortService.this) {
						if (mRemoteBinder == remoteBinder) {
							mRemoteSvc = null;
							mRemoteBinder = null;
						}
					}
				}
            		}, 0);
		} catch (Throwable t) {
			Log.w(TAG, "linkToDeath failed", t);
        	}

        mRemoteSvc = svc;
        mRemoteBinder = remoteBinder;
        Log.i(TAG, "getRemoteService: cached remote svc");
        return mRemoteSvc;
    
	}
}
