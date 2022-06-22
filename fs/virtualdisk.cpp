#include "virtualdisk.h"

bool VirtualDisk::Mount(const char* disk_path)
{
	handle = fopen(disk_path, "r+b");
	if (handle == nullptr) {
		printf("�ļ�ϵͳ������ �Ƿ��½�\n");
		return false;
	}

	// ��������
	// ֱ�Ӷ������߻���
	SuperBlock super = GetSuperBlock();

	if (super.magic != MAGIC_NUMBER
	|| super.n_inode != N_INODE
	|| super.imapstart != INODE_BITMAP_BLOCKID
	|| super.inodestart != INODE_START_BLOCK
	|| super.n_data_block != N_DATA_BLOCK
	|| super.bmapstart != DATA_BITMAP_BLOCKID
	|| super.datastart != DATA_START_BLOCK) {
		printf("�����ļ�ϵͳ��ʽ����\n");
		printf("%x %hd %hd %hd %hd %hd %hd\n", 
			super.magic,
			super.n_inode, super.imapstart, super.inodestart,
			super.n_data_block, super.bmapstart, super.datastart);

		return false;
	}

	return true;
}

void VirtualDisk::Format()
{
	// �����С�������飬bitmap������Ա

	if (handle) {  // �رյ�ǰ��
		auto ret = fclose(handle);
		ASSERT(ret == 0); 
	}

	char* buf = new char[DISK_SIZE];
	if (buf == nullptr) {
		printf("�ڴ����ʧ��\n");
		exit(1);
	}

	// ����������
	// ��ҪΪ�������û���Ϣ
	std::memset(buf, 0, BLOCK_SIZE);
	SuperBlock super;
	std::memset(&super, 0, sizeof(SuperBlock));
	super.magic = MAGIC_NUMBER;
	super.n_inode = N_INODE;
	super.imapstart = INODE_BITMAP_BLOCKID;
	super.inodestart = INODE_START_BLOCK;
	super.n_data_block = N_DATA_BLOCK;      
	super.bmapstart = DATA_BITMAP_BLOCKID;   
	super.datastart = DATA_START_BLOCK;   
	super.users[0].username = { 'r','o','o','t' };
	super.users[0].password = { 'r','o','o','t' };
	std::memmove(buf, &super, sizeof(SuperBlock));
	// bitmap��0
	std::memset(buf + INODE_BITMAP_BLOCKID * BLOCK_SIZE, 0, BLOCK_SIZE);
	std::memset(buf + DATA_BITMAP_BLOCKID * BLOCK_SIZE, 0, BLOCK_SIZE);

	handle = fopen("fs.img", "w+b");
	ASSERT(handle);
	for (int i = 0; i < NBLOCK; ++i) {
		auto ret = fwrite(buf + i * BLOCK_SIZE, sizeof(char), BLOCK_SIZE, handle);
		ASSERT(ret == 1024);
	}

	delete[] buf;
}

// ����������0  ��Ҫ�Ǹշ���һ���¿�ʱ��һ��
static void MakeZero(uint32_t blockno) {
	auto b = GetDisk().ReadBlock(blockno);

	std::memset(b->data, 0, BLOCK_SIZE);

	GetDisk().ReleaseBlock(b);
}

VirtualDisk::Buffer* VirtualDisk::FindBlock(uint32_t blockno) {
	ASSERT(blockno > 0 && blockno < NBLOCK);
	
	// �ȿ���������û��
	Buffer* ret = nullptr;
	for (auto b = head.next; b != &head; b = b->next) {
		if (b->blockno == blockno) {
			b->refs++;
			ret = b;
			break;
		}
	}
	if (ret == nullptr) {
		// Ҫ�ҿ�λ��
		for (auto b = head.prev; b != &head; b = b->prev) {
			if (b->refs == 0) {
				b->blockno = blockno;
				b->valid = 0;
				b->refs = 1;
				ret = b;
				break;
			}
		}
	}
	ASSERT(ret != nullptr); // bufferȫ����    TODO һ���Դ򿪵Ŀ鲻�ܳ�����������С���� 
	
	// �ŵ�����ͷ
	ret->next->prev = ret->prev;
	ret->prev->next = ret->next;

	ret->next = head.next;
	ret->prev = &head;

	head.next->prev = ret;
	head.next = ret;

	return ret;
}

VirtualDisk::Buffer* VirtualDisk::ReadBlock(uint32_t blockno)
{
	auto b = FindBlock(blockno);
	if (b->valid == false) {
		auto ret = std::fseek(handle, blockno * BLOCK_SIZE, SEEK_SET);
		ASSERT(ret == 0);

		auto ret2 = std::fread(b->data, sizeof(char), BLOCK_SIZE, handle); // ע�Ᵽ֤buf�Ŀռ�
		ASSERT(ret2 == 1024);

		b->valid = true;
	}
	return b;
}

void VirtualDisk::WriteBlock(const Buffer* buf)
{
	ASSERT(buf != nullptr);

	auto ret = std::fseek(handle, buf->blockno * BLOCK_SIZE, SEEK_SET);
	ASSERT(ret == 0);

	auto ret2 = std::fwrite(buf->data, sizeof(char), BLOCK_SIZE, handle);
	ASSERT(ret2 == 1024);
}

uint32_t VirtualDisk::AllocDataBlock() {
	// �����ݵ�bitmap
	// ������Ӧ�Ŀ�
	auto b = ReadBlock(DATA_BITMAP_BLOCKID);

	//�൱��1024*8�ı�� ��ûȫ��ռ����
	int row = 0;
	for (; row < N_DATA_BLOCK / 8; ++row) {
		if ((uint8_t)(b->data[row]) != 255u) {   // maybe������΢���
			for (int col = 0, mask = 1; col < 8; ++col, mask <<= 1) {
				if ((b->data[row] & mask) == 0) {
					b->data[row] |= mask;

					uint32_t blockno = row * 8 + col + DATA_START_BLOCK;
					MakeZero(blockno);
					ReleaseBlock(b);

					return blockno;
				}
			}
		}
	}

	// ����ʣ�²���������
	for (int bit = row * 8, col = 0; bit < N_DATA_BLOCK; ++bit, ++col) {
		if ((b->data[row] & (1 << col)) == 0) {
			b->data[row] |= (1 << col);

			uint32_t blockno = bit + DATA_START_BLOCK;
			MakeZero(blockno);
			ReleaseBlock(b);
			return blockno;
		}
	}

	ReleaseBlock(b);
	ASSERT(0); // û�п��п���
	return 0;
}

void VirtualDisk::FreeDataBlock(uint32_t blockno) {
	ASSERT(blockno >= DATA_START_BLOCK);
	blockno -= DATA_START_BLOCK;
	ASSERT(blockno < N_DATA_BLOCK);

	// �����ݵ�bitmap
	auto b = ReadBlock(DATA_BITMAP_BLOCKID);

	int row = blockno / 8;
	int col = blockno % 8;
	int mask = 1 << col;

	ASSERT((b->data[row] & mask) > 0); // ����ʹ�õĿ�

	b->data[row] &= ~mask; // ��λ����

	ReleaseBlock(b);
}

SuperBlock VirtualDisk::GetSuperBlock()
{
	char buf[BLOCK_SIZE];
	std::memset(buf, 0, sizeof(SuperBlock));
	auto ret = std::fseek(handle, SUPERBLOCK_ID * BLOCK_SIZE, SEEK_SET);
	ASSERT(ret == 0);
	auto ret2 = std::fread(buf, sizeof(char), BLOCK_SIZE, handle);
	ASSERT(ret2 == 1024);

	SuperBlock super;
	std::memmove(&super, buf, sizeof(SuperBlock));
	return super;
}

void VirtualDisk::WriteSuperBlock(const SuperBlock& super)
{
	char buf[BLOCK_SIZE];
	std::memset(buf, 0, BLOCK_SIZE);
	std::memmove(buf, &super, sizeof(SuperBlock));

	auto ret = std::fseek(handle, SUPERBLOCK_ID * BLOCK_SIZE, SEEK_SET);
	ASSERT(ret == 0);
	auto ret2 = std::fwrite(buf, sizeof(char), BLOCK_SIZE, handle);
	ASSERT(ret2 == 1024);

}

VirtualDisk::VirtualDisk()
{
	head.prev = &head;
	head.next = &head;
	for (int i = 0; i < BUFFER_SIZE; ++i) {
		buffer[i].next = head.next;   // ��ʼ�� ͷ�巨
		buffer[i].prev = &head;

		head.next->prev = &buffer[i];
		head.next = &buffer[i];
	}
}

void VirtualDisk::ReleaseBlock(Buffer* buf)
{
	ASSERT(buf);
	ASSERT(buf->refs >= 1);
	buf->refs--;
	if (buf->refs == 0) {
		WriteBlock(buf);  // �ӻ������б������Ķ�д�ص�����

		// ����LRU�ṹ
		// �ŵ�����β
		buf->next->prev = buf->prev;
		buf->prev->next = buf->next;

		buf->next = &head;
		buf->prev = head.next;

		head.prev->next = buf;
		head.prev = buf;

		buf->blockno = 0;
		buf->valid = 0;
	}
}


VirtualDisk& GetDisk()
{
	static VirtualDisk instance;
	return instance;
}

VirtualDisk::~VirtualDisk()
{
	// TODO �رյ�ʱ���Ƿ���Ҫ�ѻ��嶼д�������ϣ� �ƺ�����Ҫ��
	for (const auto& b : buffer) {
		if (b.valid) {
			WriteBlock(&b);
		}
	}

	int ret = fclose(handle);
	ASSERT(ret == 0);
}
