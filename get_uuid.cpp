#pragma once
#include "get_uuid.h"

std::string get_uuid(){
	// 创建一个随机UUID生成器
	boost::uuids::random_generator gen;

	// 使用生成器生成一个UUID
	boost::uuids::uuid uuid = gen();

	// 将UUID转换为字符串并返回
	return boost::uuids::to_string(uuid);
}