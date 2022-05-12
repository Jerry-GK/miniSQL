#include "buffer/lru_replacer.h"

LRUReplacer::LRUReplacer(size_t num_pages) {
  this->num_pages=num_pages;
  //set the max size of lru_list_ to num_pages
}

LRUReplacer::~LRUReplacer() = default;

bool LRUReplacer::Victim(frame_id_t *frame_id) {
  frame_id_t max_frame_id = -1;
  int max_time = -1;
  for (map<frame_id_t, int>::iterator it = lru_list_.begin(); it != lru_list_.end(); it++) {
    if(it->second>max_time)
    {
      max_time = it->second;
      max_frame_id = it->first;
    }
  }
  if(max_frame_id==-1)//empty lru_list_, can not replace
  {
    frame_id=nullptr;
    return false;
  }
  *frame_id = max_frame_id;
  lru_list_.erase(max_frame_id);//delete the victimed frame
  return true;
}

void LRUReplacer::Pin(frame_id_t frame_id) {
  if(lru_list_.find(frame_id)!=lru_list_.end())//exist
  {
    lru_list_.erase(frame_id);
    for(map<frame_id_t, int>::iterator it=lru_list_.begin();it!=lru_list_.end();it++)
    {
      it->second++;//increase old factor of frames in list
    }
  }
}

void LRUReplacer::Unpin(frame_id_t frame_id) {
  if(lru_list_.find(frame_id)==lru_list_.end())//not exist
  {
    lru_list_.insert(make_pair(frame_id, 0));
    for(map<frame_id_t, int>::iterator it=lru_list_.begin();it!=lru_list_.end();it++)
    {
      it->second++;//increase old factor of frames in list
    }
  }
}

size_t LRUReplacer::Size() {
  return lru_list_.size();
}