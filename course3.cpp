#include "libfluid_msg/fluid/of13msg.hh"
#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <unordered_map>


using namespace fluid_msg;

#define MIN_INTERSECTION_LEN 32

template<typename T>
std::string toBinary(const T& t)
{
  std::string s = "";
  int n = sizeof(T)*8;
  for(int i=n-1; i>=0; i--)
  {
    s += (t & (1 << i))?"1":"0";
  }
  return s;
}

class bucket_comparisable{
public:
	bucket_comparisable(){}
	bucket_comparisable(of13::Bucket b):buc(b){
		auto actSet = buc.get_actions();
		existing_actions = 0;
		std::set<Action*, comp_action_set_order> actions = actSet.action_set();
		for(auto it : actions){
			switch (it->type()) {
		        case (of13::OFPAT_OUTPUT): {
		            existing_actions += 1;
		            continue;
		        }
		        case (of13::OFPAT_COPY_TTL_OUT): {
		            existing_actions += (1 << 1);
		            continue;
		        }
		        case (of13::OFPAT_COPY_TTL_IN): {
		            existing_actions += (1 << 2);
		            continue;
		        }
		        case (of13::OFPAT_SET_MPLS_TTL): {
		            existing_actions += (1 << 3);
		            continue;
		        }
		        case (of13::OFPAT_DEC_MPLS_TTL): {
		            existing_actions += (1 << 4);
		            continue;
		        }
		        case (of13::OFPAT_PUSH_VLAN): {
		            existing_actions += (1 << 5);
		            continue;
		        }
		        case (of13::OFPAT_POP_VLAN): {
		            existing_actions += (1 << 6);
		            continue;
		        }
		        case (of13::OFPAT_PUSH_MPLS): {
		            existing_actions += (1 << 7);
		            continue;
		        }
		        case (of13::OFPAT_POP_MPLS): {
		            existing_actions += (1 << 8);
		            continue;
		        }
		        case (of13::OFPAT_SET_QUEUE): {
		            existing_actions += (1 << 9);
		            continue;
		        }
		        case (of13::OFPAT_GROUP): {
		            existing_actions += (1 << 10);
		            continue;
		        }
		        case (of13::OFPAT_SET_NW_TTL): {
		            existing_actions += (1 << 11);
		            continue;
		        }
		        case (of13::OFPAT_DEC_NW_TTL): {
		            existing_actions += (1 << 12);
		            continue;
		        }
		        case (of13::OFPAT_SET_FIELD): {
		            existing_actions += (1 << 13);
		            continue;
		        }
		        case (of13::OFPAT_PUSH_PBB): {
		            existing_actions += (1 << 14);
		            continue;
		        }
		        case (of13::OFPAT_POP_PBB): {
		            existing_actions += (1 << 15);
		            continue;
		        }
		        case (of13::OFPAT_EXPERIMENTER): {
		            existing_actions += (1 << 16);
		            continue;
		        }
		    }
		}
	}
	bucket_comparisable(const bucket_comparisable& r ):buc(r.buc),existing_actions(r.existing_actions),visualize(r.visualize){

	}
	inline bucket_comparisable& operator=( const bucket_comparisable& r){
		this->existing_actions = r.existing_actions;
		this->buc = r.buc;
		this->visualize = r.visualize;
		return * this;
	}
	
	uint32_t existing_actions;
	of13::Bucket buc;
	std::string visualize;

};
bool operator==(const bucket_comparisable& a,const  bucket_comparisable& b){
	return (a.existing_actions == b.existing_actions)&&(a.buc == b.buc); 
};





bucket_comparisable create_bucket_link(uint group_id){
	of13::Bucket b1;
    b1.add_action(new of13::GroupAction(group_id));
    bucket_comparisable a(b1);
    a.visualize = std::to_string(group_id);
    return a;
}

struct cmp {
    bool operator()(const bucket_comparisable& a,const  bucket_comparisable& b) const {
        if (a.existing_actions < b.existing_actions){
			return true;
		}
		if (a == b){
			return false;
		}
		if ((a).existing_actions == b.existing_actions){
			auto actSet = (a.buc.get_actions());
			std::set<Action*, comp_action_set_order> actions = actSet.action_set();
			auto actSet2 = b.buc.get_actions();
			std::set<Action*, comp_action_set_order> actions2 = actSet2.action_set();
			auto it1 = actions.begin();
			auto it2 = actions2.begin();
			for (;it1 != actions.end();){
				switch ((*it1)->type()) {
		        case (of13::OFPAT_OUTPUT): {
		            auto tmp1 = dynamic_cast<of13::OutputAction*> (*it1);
		            auto tmp2 = dynamic_cast<of13::OutputAction*> (*it2);
		            if (tmp1->port() < tmp2->port()){
		            	return true;
		            }
		            if (tmp1->port() > tmp2->port()){
		            	return false;
		            }
		            it1++;
		            it2++;
		            continue;
		        }
		        case (of13::OFPAT_COPY_TTL_OUT): {
		            
		            it1++;
		            it2++;
		            continue;
		        }
		        case (of13::OFPAT_COPY_TTL_IN): {
		            
		            it1++;
		            it2++;
		            continue;
		        }
		        case (of13::OFPAT_SET_MPLS_TTL): {
		            auto tmp1 = dynamic_cast<of13::SetMPLSTTLAction*> (*it1);
		            auto tmp2 = dynamic_cast<of13::SetMPLSTTLAction*> (*it2);
		            if (tmp1->mpls_ttl() < tmp2->mpls_ttl()){
		            	return true;
		            }
		            if (tmp1->mpls_ttl() > tmp2->mpls_ttl()){
		            	return false;
		            }
		            it1++;
		            it2++;
		            continue;
		        }
		        case (of13::OFPAT_DEC_MPLS_TTL): {
		            
		            it1++;
		            it2++;
		            continue;
		        }
		        case (of13::OFPAT_PUSH_VLAN): {
		            auto tmp1 = dynamic_cast<of13::PushVLANAction*> (*it1);
		            auto tmp2 = dynamic_cast<of13::PushVLANAction*> (*it2);
		            if (tmp1->ethertype() < tmp2->ethertype()){
		            	return true;
		            }
		            if (tmp1->ethertype() > tmp2->ethertype()){
		            	return false;
		            }
		            it1++;
		            it2++;
		            continue;
		        }
		        case (of13::OFPAT_POP_VLAN): {
		            
		            it1++;
		            it2++;
		            continue;
		        }
		        case (of13::OFPAT_PUSH_MPLS): {
		            auto tmp1 = dynamic_cast<of13::PushMPLSAction*> (*it1);
		            auto tmp2 = dynamic_cast<of13::PushMPLSAction*> (*it2);
		            if (tmp1->ethertype() < tmp2->ethertype()){
		            	return true;
		            }
		            if (tmp1->ethertype() > tmp2->ethertype()){
		            	return false;
		            }
		            it1++;
		            it2++;
		            continue;
		        }
		        case (of13::OFPAT_POP_MPLS): {
		            
		            it1++;
		            it2++;
		            continue;
		        }
		        case (of13::OFPAT_SET_QUEUE): {
		            auto tmp1 = dynamic_cast<of13::SetQueueAction*> (*it1);
		            auto tmp2 = dynamic_cast<of13::SetQueueAction*> (*it2);
		            if (tmp1->queue_id() < tmp2->queue_id()){
		            	return true;
		            }
		            if (tmp1->queue_id() > tmp2->queue_id()){
		            	return false;
		            }
		            it1++;
		            it2++;
		            continue;
		            it1++;
		            it2++;
		            continue;
		        }
		        case (of13::OFPAT_GROUP): {
		            auto tmp1 = dynamic_cast<of13::GroupAction*> (*it1);
		            auto tmp2 = dynamic_cast<of13::GroupAction*> (*it2);
		            if (tmp1->group_id() < tmp2->group_id()){
		            	return true;
		            }
		            if (tmp1->group_id() > tmp2->group_id()){
		            	return false;
		            }
		            it1++;
		            it2++;
		            continue;
		            it1++;
		            it2++;
		            continue;
		        }
		        case (of13::OFPAT_SET_NW_TTL): {
		            auto tmp1 = dynamic_cast<of13::SetNWTTLAction*> (*it1);
		            auto tmp2 = dynamic_cast<of13::SetNWTTLAction*> (*it2);
		            if (tmp1->nw_ttl() < tmp2->nw_ttl()){
		            	return true;
		            }
		            if (tmp1->nw_ttl() > tmp2->nw_ttl()){
		            	return false;
		            }
		            it1++;
		            it2++;
		            continue;
		        }
		        case (of13::OFPAT_DEC_NW_TTL): {
		            
		            it1++;
		            it2++;
		            continue;
		        }
		        case (of13::OFPAT_SET_FIELD): {
		            it1++;
		            it2++;
		            continue;
		        }
		        case (of13::OFPAT_PUSH_PBB): {
		            auto tmp1 = dynamic_cast<of13::PushPBBAction*> (*it1);
		            auto tmp2 = dynamic_cast<of13::PushPBBAction*> (*it2);
		            if (tmp1->ethertype() < tmp2->ethertype()){
		            	return true;
		            }
		            if (tmp1->ethertype() > tmp2->ethertype()){
		            	return false;
		            }
		            it1++;
		            it2++;
		            continue;
		        }
		        case (of13::OFPAT_POP_PBB): {
		           
		            it1++;
		            it2++;
		            continue;
		        }
		        case (of13::OFPAT_EXPERIMENTER): {
		            
		            it1++;
		            it2++;
		            continue;
		        }
		    }
			}	
		}
		return false;
    }

};

uint get_buckets_len(std::set<bucket_comparisable,cmp>& intersection){
	uint len = 0;
	for(auto it : intersection){
		len += it.buc.len();
	}
	return len;
}

bool same_intersections(std::set<bucket_comparisable,cmp>&l , std::set<bucket_comparisable,cmp>& r){
	auto it1 = l.begin();
	auto it2 = r.begin();
	if(l.size()!=r.size()){
		return false;
	}
	for(;it1 != l.end();){
	    if ((*it1)==(*it2)){
	    	it1++;
	    	it2++;
	    	continue;
	    }
	    return false;
	}
	return true;
}

    bool comp1(const bucket_comparisable& a,const  bucket_comparisable& b){
        if (a.existing_actions < b.existing_actions){
			return true;
		}
		if (a == b){
			return false;
		}
		if ((a).existing_actions == b.existing_actions){
			auto actSet = (a.buc.get_actions());
			std::set<Action*, comp_action_set_order> actions = actSet.action_set();
			auto actSet2 = b.buc.get_actions();
			std::set<Action*, comp_action_set_order> actions2 = actSet2.action_set();
			auto it1 = actions.begin();
			auto it2 = actions2.begin();
			for (;it1 != actions.end();){
				switch ((*it1)->type()) {
		        case (of13::OFPAT_OUTPUT): {
		            auto tmp1 = dynamic_cast<of13::OutputAction*> (*it1);
		            auto tmp2 = dynamic_cast<of13::OutputAction*> (*it2);
		            if (tmp1->port() < tmp2->port()){
		            	return true;
		            }
		            if (tmp1->port() > tmp2->port()){
		            	return false;
		            }
		            it1++;
		            it2++;
		            continue;
		        }
		        case (of13::OFPAT_COPY_TTL_OUT): {
		            
		            it1++;
		            it2++;
		            continue;
		        }
		        case (of13::OFPAT_COPY_TTL_IN): {
		            
		            it1++;
		            it2++;
		            continue;
		        }
		        case (of13::OFPAT_SET_MPLS_TTL): {
		            auto tmp1 = dynamic_cast<of13::SetMPLSTTLAction*> (*it1);
		            auto tmp2 = dynamic_cast<of13::SetMPLSTTLAction*> (*it2);
		            if (tmp1->mpls_ttl() < tmp2->mpls_ttl()){
		            	return true;
		            }
		            if (tmp1->mpls_ttl() > tmp2->mpls_ttl()){
		            	return false;
		            }
		            it1++;
		            it2++;
		            continue;
		        }
		        case (of13::OFPAT_DEC_MPLS_TTL): {
		            
		            it1++;
		            it2++;
		            continue;
		        }
		        case (of13::OFPAT_PUSH_VLAN): {
		            auto tmp1 = dynamic_cast<of13::PushVLANAction*> (*it1);
		            auto tmp2 = dynamic_cast<of13::PushVLANAction*> (*it2);
		            if (tmp1->ethertype() < tmp2->ethertype()){
		            	return true;
		            }
		            if (tmp1->ethertype() > tmp2->ethertype()){
		            	return false;
		            }
		            it1++;
		            it2++;
		            continue;
		        }
		        case (of13::OFPAT_POP_VLAN): {
		            
		            it1++;
		            it2++;
		            continue;
		        }
		        case (of13::OFPAT_PUSH_MPLS): {
		            auto tmp1 = dynamic_cast<of13::PushMPLSAction*> (*it1);
		            auto tmp2 = dynamic_cast<of13::PushMPLSAction*> (*it2);
		            if (tmp1->ethertype() < tmp2->ethertype()){
		            	return true;
		            }
		            if (tmp1->ethertype() > tmp2->ethertype()){
		            	return false;
		            }
		            it1++;
		            it2++;
		            continue;
		        }
		        case (of13::OFPAT_POP_MPLS): {
		            
		            it1++;
		            it2++;
		            continue;
		        }
		        case (of13::OFPAT_SET_QUEUE): {
		            auto tmp1 = dynamic_cast<of13::SetQueueAction*> (*it1);
		            auto tmp2 = dynamic_cast<of13::SetQueueAction*> (*it2);
		            if (tmp1->queue_id() < tmp2->queue_id()){
		            	return true;
		            }
		            if (tmp1->queue_id() > tmp2->queue_id()){
		            	return false;
		            }
		            it1++;
		            it2++;
		            continue;
		            it1++;
		            it2++;
		            continue;
		        }
		        case (of13::OFPAT_GROUP): {
		            auto tmp1 = dynamic_cast<of13::GroupAction*> (*it1);
		            auto tmp2 = dynamic_cast<of13::GroupAction*> (*it2);
		            if (tmp1->group_id() < tmp2->group_id()){
		            	return true;
		            }
		            if (tmp1->group_id() > tmp2->group_id()){
		            	return false;
		            }
		            it1++;
		            it2++;
		            continue;
		            it1++;
		            it2++;
		            continue;
		        }
		        case (of13::OFPAT_SET_NW_TTL): {
		            auto tmp1 = dynamic_cast<of13::SetNWTTLAction*> (*it1);
		            auto tmp2 = dynamic_cast<of13::SetNWTTLAction*> (*it2);
		            if (tmp1->nw_ttl() < tmp2->nw_ttl()){
		            	return true;
		            }
		            if (tmp1->nw_ttl() > tmp2->nw_ttl()){
		            	return false;
		            }
		            it1++;
		            it2++;
		            continue;
		        }
		        case (of13::OFPAT_DEC_NW_TTL): {
		            
		            it1++;
		            it2++;
		            continue;
		        }
		        case (of13::OFPAT_SET_FIELD): {
		            //todo how to work
		            it1++;
		            it2++;
		            continue;
		        }
		        case (of13::OFPAT_PUSH_PBB): {
		            auto tmp1 = dynamic_cast<of13::PushPBBAction*> (*it1);
		            auto tmp2 = dynamic_cast<of13::PushPBBAction*> (*it2);
		            if (tmp1->ethertype() < tmp2->ethertype()){
		            	return true;
		            }
		            if (tmp1->ethertype() > tmp2->ethertype()){
		            	return false;
		            }
		            it1++;
		            it2++;
		            continue;
		        }
		        case (of13::OFPAT_POP_PBB): {
		           
		            it1++;
		            it2++;
		            continue;
		        }
		        case (of13::OFPAT_EXPERIMENTER): {
		            
		            it1++;
		            it2++;
		            continue;
		        }
		    }
			}


			
		}
		return false;
    }

void set_intersection(std::set<bucket_comparisable,cmp>::iterator first1, std::set<bucket_comparisable,cmp>::iterator last1,
                                    std::set<bucket_comparisable,cmp>::iterator first2, std::set<bucket_comparisable,cmp>::iterator last2,
                                    std::set<bucket_comparisable,cmp>& d_first)
{
    while (first1 != last1 && first2 != last2) {
        if (comp1(*first1, *first2)) {
            ++first1;
        } else {
            if (!comp1(*first2, *first1)) {
                bucket_comparisable a(*(first1));
                //std::cout << (*first1).visualize<<std::endl;
                d_first.emplace(a);
                first1++;
            }
            ++first2;
        }
    }
    //    return d_first;
}




uint get_max_weight_intersection(std::map<uint,std::pair<uint,std::set<bucket_comparisable,cmp>>> intersections, std::map<uint,std::vector<uint>> same_intersec_refl){
	uint max_size = 0;
	uint max_id = -1;
	for(auto it:intersections){
		uint curr_size = it.second.first * same_intersec_refl[it.first].size();
		if (curr_size > max_size){
			max_size = curr_size;
		    max_id = it.first;
		}
	}
	return max_id;
}

static int new_group_id = 50000;
void print_existing_groups(std::map<uint,std::set<bucket_comparisable,cmp>> existing_groups){
	std::cout << "=================" << std::endl;
	for(auto it1 : existing_groups){
		std::cout << it1.first << "    ";
		for(auto it2 : it1.second){
			std::cout << it2.visualize << " ";
		}
		std::cout << std::endl;
	}
	std::cout << "=================" << std::endl;

}
void print_intersections(std::map<uint,std::pair<uint,std::set<bucket_comparisable,cmp>>>  existing_groups){
	std::cout << "=================" << std::endl;
	for(auto it1 : existing_groups){
		std::cout << it1.first << "    ";
		for(auto it2 : it1.second.second){
			std::cout << it2.visualize << " ";
		}
		std::cout << std::endl;
	}
	std::cout << "=================" << std::endl;

}
void print_current_group(std::set<bucket_comparisable,cmp> group){
	std::cout <<  "-------------------" << std::endl;
	for(auto it : group){
		std::cout << it.visualize << " ";
	}
	std::cout << std::endl << "-------------------" << std::endl;
}


void add_group(std::map<uint,std::set<bucket_comparisable,cmp>>& existing_groups, std::set<bucket_comparisable,cmp>& new_group, uint group_id){
	if(existing_groups.empty()){
		existing_groups[group_id] = new_group;
		return;
	}
	std::set<bucket_comparisable,cmp> max;
	uint curr_max_len;
	std::vector <uint> max_intersection_groups;
	std::map<uint,std::pair<uint,std::set<bucket_comparisable,cmp>>> intersections;//отсортировать перед поиском
	uint counter = 0;
	std::map<uint,std::vector<uint>> same_intersec_refl;
	for(auto it : existing_groups){
		std::set<bucket_comparisable,cmp> tmp;
	    set_intersection(it.second.begin(),it.second.end(),new_group.begin(),new_group.end(),tmp);
	    if((!tmp.empty()) && (get_buckets_len(tmp) < MIN_INTERSECTION_LEN)){
	    	//std::cout << "!";
	    	continue;
	    }
	    if(tmp.empty()){
	    	//std::cout << "!";
	    	continue;
	    }
	    bool flag = true;
	    for(auto it1: intersections){
	    	if(same_intersections(it1.second.second, tmp)){
	    		same_intersec_refl[it1.first].push_back(it.first);
	    		flag = false;
	    		break;
	    	}
	    }
	    if(flag){//not found such intersection so far
	    	uint curr_len = get_buckets_len(tmp);
	    	same_intersec_refl[counter].push_back(it.first);
	    	intersections[counter] = std::make_pair(curr_len,tmp);
	    	counter++;
	    }
	}
	if(!intersections.empty()){
			//std::cout << "!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
			//print_intersections(intersections);
			//std::cout << intersections.size() <<"!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
    }
	while(intersections.size() != 0){
		if (intersections.size() == 1){
			if (intersections[0].second.empty()){
				break;
			}
		}
		int  tmp_new_group_id = new_group_id;
		//print_existing_groups(existing_groups);
		uint curr_intersection = get_max_weight_intersection(intersections,same_intersec_refl);
		for(auto it : same_intersec_refl[curr_intersection]){
			for(auto it1 : intersections[curr_intersection].second){
				existing_groups[it].erase(it1);
			}
			if ((it >= 50000) && (existing_groups[it].empty())){
				tmp_new_group_id = new_group_id;
				new_group_id = it;

			}
			existing_groups[it].insert(create_bucket_link(new_group_id));
		}
		for(auto it: intersections[curr_intersection].second){
			new_group.erase(it);
		}
		new_group.insert(create_bucket_link(new_group_id));
		existing_groups[new_group_id] = intersections[curr_intersection].second;		
		for(auto it : intersections){
			for(auto it1 : intersections[curr_intersection].second){
				it.second.second.erase(it1);
			}
		}
		intersections.erase(curr_intersection);
		same_intersec_refl.erase(curr_intersection);
		for(auto it : intersections){
			it.second.first = get_buckets_len(it.second.second);
			if(it.second.first < MIN_INTERSECTION_LEN){
				//std::cout << "erase";
				intersections.erase(it.first);
			}
		}
		if(new_group_id == tmp_new_group_id){
		    new_group_id++;
		}else{
			new_group_id = tmp_new_group_id;
		}    

	}
	existing_groups[group_id] = new_group;    
	
}

bucket_comparisable random_bucket_generator(){
	of13::Bucket res;
	uint existing_actions = 0 ;
	bool flag = false;
	if (std::rand()%2 == 0){	
		bool flag = true;
		existing_actions += 1;
		res.add_action(new of13::OutputAction(std::rand()%32 + 1, 0));// one of 32 ports
	}
	if(std::rand()%100 < 1){
		bool flag = true;
		existing_actions += (1<<1);
		res.add_action(new of13::CopyTTLOutAction());
	}           
	if(std::rand()%100 < 1){
		bool flag = true;
		existing_actions += (1<<2);
		res.add_action(new of13::CopyTTLInAction());
	}
	if(std::rand()%100 < 1){
		bool flag = true;
		existing_actions +=(1<<3);
		res.add_action(new of13::SetMPLSTTLAction(std::rand()%2+1));//one out of 2 times
	}
	if(std::rand()%100 < 1){
		bool flag = true;
		existing_actions += (1 << 4);
		res.add_action(new of13::DecMPLSTTLAction());
	}
	if(std::rand()%10 < 1){
		bool flag = true;
		existing_actions += (1 << 5);
		res.add_action(new of13::PushVLANAction(std::rand()%10 + 1));//one out of 10 VLANs
	}
	if(std::rand()%10 < 1){
		bool flag = true;
		existing_actions += (1 << 6);
		res.add_action(new of13::PopVLANAction());
	}
	if(std::rand()%100 < 1){
		bool flag = true;
		existing_actions += (1 << 7);
		res.add_action(new of13::PushMPLSAction(std::rand()%10 + 1));//one out of 10 MPLSs
	}
	if(std::rand()%100 < 1){
		bool flag = true;
		existing_actions += (1 << 8);
		res.add_action(new of13::PopMPLSAction());
	}
	if(std::rand()%100 < 1){
		bool flag = true;
		existing_actions += (1 << 9);
		res.add_action(new of13::SetQueueAction());
   	}        
	if(std::rand()%2 < 1){
		bool flag = true;
		existing_actions += (1 << 10);
		res.add_action(new of13::GroupAction(std::rand()%10 + 1));//one out of 10 groups
	} 
	if(std::rand()%100 < 1){
		bool flag = true;
		existing_actions += (1 << 11);
		res.add_action(new of13::SetNWTTLAction(std::rand()%2 + 1));//one out of 2 times
	}
	if(std::rand()%100 < 1){
		bool flag = true;
		existing_actions += (1 << 12);
		res.add_action(new of13::DecNWTTLAction());
	}
	if(std::rand()%100 <1){
		bool flag = true;
		existing_actions += (1 << 14);
		res.add_action(new of13::PushPBBAction(std::rand()%10 + 1));//one  out of 10 PBBs
	}
	if(std::rand()%100 < 1){
		bool flag = true;
		existing_actions += (1 << 15);
		res.add_action(new of13::PopPBBAction());
	}
	if(!flag){
		bool flag = true;
		existing_actions += (1 << 10);
		res.add_action(new of13::GroupAction(std::rand()%10 + 1));
	}
	bucket_comparisable rres(res);
	rres.visualize = std::to_string((existing_actions));
    return rres;

}

void random_group_generator(std::set<bucket_comparisable, cmp> & res){
	int bucket_to_generate = (std::rand()%20)+20;//from 8 to 20 buckets will be generated for this particular group
	for (auto it = 0; it < bucket_to_generate; it ++){
		res.emplace(random_bucket_generator());
	}
}


int main(int argc, const char** argv){//getbuckets, check 4 equallity, add to struct
	std::unordered_map<std::string, std::string> cmdLineParams;
    for(int i=0; i<argc; i++){
        std::string key(argv[i]);
        if(key.size() > 0 && key[0]=='-'){
            if(i != argc-1){ // not last argument
                cmdLineParams[key] = argv[i+1];
                i++;
            }
            else
                cmdLineParams[key] = "";
        }
    }
    if(cmdLineParams.find("-test") != cmdLineParams.end() ){
		std::map<uint,std::set<bucket_comparisable,cmp>> existing_groups;
		of13::Bucket b1;
	    b1.add_action(new of13::GroupAction(1));
	    of13::Bucket b2;
	    b2.add_action(new of13::GroupAction(2));
	    of13::Bucket b3;
	    b3.add_action(new of13::OutputAction(1, 0));
	    bucket_comparisable a(b1);
	    a.visualize = "A ";
	    bucket_comparisable b(b2);
	    b.visualize = "B ";
	    bucket_comparisable c(b3);
	    c.visualize = "C ";
	    std::set<bucket_comparisable,cmp> ab;
	    std::set<bucket_comparisable,cmp> ac;
	    ab.emplace(c);
	    ab.emplace(a);
	    ac.emplace(b);
	    ac.emplace(c);
	    existing_groups[2].emplace(a);
	    existing_groups[1].emplace(b);
	    existing_groups[1].emplace(c);
	    print_existing_groups(existing_groups);
	    add_group(existing_groups,ab,3);
	    print_existing_groups(existing_groups);
	    return 0;
	    //std::cout << (ac == ab);
	}
	if(cmdLineParams.find("-time")!= cmdLineParams.end()){
		std::string str = cmdLineParams["-time"];
		std::srand(time(0));
		std::map<uint,std::set<bucket_comparisable,cmp>> existing_groups;
		int native_bucket_length = 0;
		int final_bucket_length = 0;
	    for (auto it = 0; it < (std::atoi(str.c_str())); it++){    
	        std::set<bucket_comparisable,cmp> tmp;
	        random_group_generator(tmp);
	        //print_current_group(tmp);
	        native_bucket_length += get_buckets_len(tmp); 
	        add_group(existing_groups,tmp,it);
	        std::cout << it << std::endl;
       	    //print_existing_groups(existing_groups);
    	}    
    	for(auto it : existing_groups){
    		final_bucket_length += get_buckets_len(it.second);
    	}
    	std::cout << "native_bucket_length = " << native_bucket_length << " final_bucket_length = "<< final_bucket_length; 
    }
}

