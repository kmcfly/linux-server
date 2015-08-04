/////////////////////////////////////////////////////////////////////
//检测flash中的网卡类型和/etc/init.d/NetChip.dat中的网卡类型是否一致如果不一致就把flash中的写到NetChip.dat中
//通过后门修改网卡类型时写网卡类型到flash和NetChip.dat文件中
#if defined(__TDFH__) || defined(__TDNXP__) || defined(__CHIPTI__) || defined(__CHIPGM__)
const char NIC_TYPE_FILE[] = "/mnt/mtd/NetChip.dat";
#else
const char NIC_TYPE_FILE[] = "/etc/init.d/NetChip.dat";
#endif
typedef enum _nic_type_
{
	NIC_TYPE_T = 0,		//1000M网卡
	NIC_TYPE_H = 1,		//100M网卡
	NIC_TYPE_END = 2,
}NIC_TYPE;

class CNICTypeMan
{
public:
	CNICTypeMan();
	~CNICTypeMan();

	static bool ChangeNICType(NIC_TYPE nicType);
	static bool NICTypeIsNotSame(unsigned long productID);
protected:
private:
	static bool WriteNICTypeToFile(NIC_TYPE nicType);
	static bool ReadNICTypeFromFile(NIC_TYPE &nicType);
	static inline NIC_TYPE GetRealNICType(unsigned long productID);
};

