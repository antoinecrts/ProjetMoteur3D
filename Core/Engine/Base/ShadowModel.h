#ifndef __SHADOW_MODEL_H
#define __SHADOW_MODEL_H

//#include "Buffers/GPUBuffer.h"
#include "GPUResources/Buffers/GPUBuffer.h"



class Node;


class ShadowModel
{
public:
	ShadowModel(bool c, bool r,Node * n);
	~ShadowModel();
	bool castShadow();
	bool receiveShadow();
	void setCastShadow(bool b);
	void setReceiveShadow(bool b);
	GPUBuffer *getLsp2SSBO();
	GPUBuffer *getRoots();
	void initSSBO();
private:
	bool m_cast;
	bool m_receive;
	GPUBuffer * m_nodes;
	GPUBuffer * m_roots;
	unsigned int m_BufferMaxSize;
	Node *dad;
	bool inited;
};
#endif
