#pragma once

enum class CONSTANT_BUFFER_TYPE : uint8
{
	GLOBAL,			// ���� �����͸� ������ ���� ����(Light ����)
	TRANSFORM,
	MATERIAL,
	END
};

enum
{
	CONSTANT_BUFFER_COUNT = static_cast<uint8>(CONSTANT_BUFFER_TYPE::END)
};

class ConstantBuffer
{
public:
	ConstantBuffer();
	~ConstantBuffer();

	// � �������Ϳ� ������ �Ǿ� �ִ���
	void Init(CBV_REGISTER reg, uint32 size, uint32 count);

	void Clear();
	void PushGraphicsData(void* buffer, uint32 size);
	void SetGraphicsGlobalData(void* buffer, uint32 size);
	void PushComputeData(void* buffer, uint32 size);

	D3D12_GPU_VIRTUAL_ADDRESS GetGpuVirtualAddress(uint32 index);
	D3D12_CPU_DESCRIPTOR_HANDLE GetCpuHandle(uint32 index);

private:
	void CreateBuffer();
	void CreateView();

private:
	ComPtr<ID3D12Resource>	_cbvBuffer;
	BYTE*					_mappedBuffer = nullptr;	// �����͸� memcpy�ϱ� ���� ����
	uint32					_elementSize = 0;			// ���۸� � ������ - �� ����� ũ��
	uint32					_elementCount = 0;			// ���۸� � ������ - �� ����� ����

	ComPtr<ID3D12DescriptorHeap>	_cbvHeap;
	D3D12_CPU_DESCRIPTOR_HANDLE		_cpuHandleBegin = {};		// ���� �ڵ� �ּ�
	uint32							_handleIncrementSize = 0;	// �� ĭ�� ���ؾ� ���� �ڵ�� �Ѿ��(�� �ڵ鰣 ����)

	uint32					_currentIndex = 0;			// ���۸� ������ ����ߴ���

	CBV_REGISTER			_reg = {};
};

