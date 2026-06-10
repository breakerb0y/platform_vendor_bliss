package custom.hardware.gpportservice;

interface IGpPortService {
	int opendrawer(int drawer_number);
	byte getstatus();
	int write(in String path, in byte[] data, int size, int addr);
	int read(in String path, out byte[] buffer, int size, int addr);
}
