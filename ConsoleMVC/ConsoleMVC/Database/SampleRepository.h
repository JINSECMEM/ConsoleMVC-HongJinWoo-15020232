#pragma once
#include "IRepository.h"
#include "../Models/Sample.h"
#include <map>

class SampleRepository : public IRepository<Sample> {
public:
    SampleRepository();  // 시료 마스터 데이터 초기 적재

    void                Save(const Sample& s)           override;
    Sample              FindById(const std::string& id) override;
    std::vector<Sample> FindAll()                       override;
    void                Update(const Sample& s)         override;
    void                Delete(const std::string& id)   override;

private:
    std::map<std::string, Sample> store_;
};
