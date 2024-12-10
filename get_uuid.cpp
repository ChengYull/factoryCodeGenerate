#pragma once
#include "get_uuid.h"

std::string get_uuid(){
	// ����һ�����UUID������
	boost::uuids::random_generator gen;

	// ʹ������������һ��UUID
	boost::uuids::uuid uuid = gen();

	// ��UUIDת��Ϊ�ַ���������
	return boost::uuids::to_string(uuid);
}