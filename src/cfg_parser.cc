#include "cfg_parser.h"
#include <string>
#include "Common.h"

cfg_parser::cfg_parser()
{
    boost::property_tree::ini_parser::read_ini("cfg/cfg.ini", tree);
}

cfg_parser::~cfg_parser()
{
}

int cfg_parser::get_settings(std::string &ip, std::string &path, std::string &fileName, std::string &signature)
{
    try
    {
        ip = tree.get <std::string>("Settings.Destination Crio IP");
        path = tree.get <std::string>("Settings.Path");
        fileName = tree.get <std::string>("Settings.Bitfile Name");
        signature = tree.get <std::string>("Settings.Signature");
    }
    catch(const boost::property_tree::ptree_error &e)
    {
        cout << e.what() << endl;
        return -1;
    }
    return 0;
}

int cfg_parser::get_bimaps(bm_type *bi_map, bm_address_type * bi_address_map)
{
    for (const std::pair<std::string, boost::property_tree::ptree> &bi_address_tree : tree.get_child("BIAddresses"))
    {
        bi_address_map->insert( bm_address_type::value_type( (bi_address_tree.first.c_str()) ,
                                                              strtoul(bi_address_tree.second.get_value<std::string>().c_str(), NULL, 16) ));
        for (const std::pair<std::string, boost::property_tree::ptree> &bi : tree.get_child(bi_address_tree.first))
            bi_map->insert( bm_type::value_type( atol(bi.first.c_str()) , bi.second.get_value<std::string>() ));
    }

    /* Print bimap
    for( bm_type::const_iterator iter = myBimap->begin(), iend = myBimap->end(); iter != iend; ++iter )
        std::cout << iter->left << " <--> " << iter->right << std::endl; */

    return 0;
}
