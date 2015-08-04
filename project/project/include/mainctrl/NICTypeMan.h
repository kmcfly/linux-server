/////////////////////////////////////////////////////////////////////
//���flash�е��������ͺ�/etc/init.d/NetChip.dat�е����������Ƿ�һ�������һ�¾Ͱ�flash�е�д��NetChip.dat��
//ͨ�������޸���������ʱд�������͵�flash��NetChip.dat�ļ���
#if defined(__TDFH__) || defined(__TDNXP__) || defined(__CHIPTI__) || defined(__CHIPGM__)
const char NIC_TYPE_FILE[] = "/mnt/mtd/NetChip.dat";
#else
const char NIC_TYPE_FILE[] = "/etc/init.d/NetChip.dat";
#endif
typedef enum _nic_type_
{
	NIC_TYPE_T = 0,		//1000M����
	NIC_TYPE_H = 1,		//100M����
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

