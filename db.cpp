#include "db.h"

namespace clock_in
{
DB::DB()
{
    // 新建/打开数据库
    options_.create_if_missing = true;
    leveldb::Status status = leveldb::DB::Open(options_, "db", &db_);
}

DB::~DB()
{
    delete db_;
}

bool DB::Put(GridInfo &grid_info)
{
    std::string key(std::to_string(grid_info.date));
    leveldb::WriteOptions write_options;
    leveldb::Slice value((char *)&grid_info, sizeof(GridInfo));
    leveldb::Status status = db_->Put(write_options, key, value);
    return status.ok();
}

bool DB::Get(GridInfo &grid_info)
{
    std::string key(std::to_string(grid_info.date));
    std::string value;
    leveldb::Status status = db_->Get(leveldb::ReadOptions(), key, &value);
    if (status.ok())
    {
        memcpy_s(&grid_info, sizeof(GridInfo), value.c_str(), sizeof(GridInfo));
    }
    else
    {
        //std::cout << "Key: " << key << " can not find!" << std::endl;
        return false;
    }
    return true;
}

bool DB::GetFirstData(GridInfo &grid_info)
{
    leveldb::Slice value;
    leveldb::Iterator *iterator = db_->NewIterator(leveldb::ReadOptions());
    iterator->SeekToFirst();
    if (iterator->Valid())
    {
        value = iterator->value();
    }
    if (value.size() != sizeof(GridInfo))
    {
        std::cout << "GetFirstData: Check failed!" << std::endl;
        delete iterator;
        return false;
    }
    memcpy_s(&grid_info, sizeof(GridInfo), value.data(), sizeof(GridInfo));
    delete iterator;
    return true;
}

}