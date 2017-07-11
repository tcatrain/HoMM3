#include "Resource/Def.hpp"
#include "Compression/ZHelper.hpp"

namespace HoMM3
{
    namespace Resource
    {
        /// <summary>Outstream operator overload used for writing</summary>
        /// <param name="os">Output stream to write in by reference</param>
        /// <param name="resource">DEF resource object by reference</param>
        /// <returns>The outstream given in input</returns>
        void Def::Dump_(std::ostream& os) const
        {
            
        }
        
    	/// <summary>
        /// Constructor of the class HoMM3::Resource::Def. Opens the input file stream
        /// and parses the file to locate content.
        /// </summary>
        /// <param name="path">Path of the DEF file to load</param>
        Def::Def(const std::string& path) : AResource(path)
        {
        }
    	
        /// <summary>
        /// Destructor if the class HoMM3::Resource::Def.
        /// </summary>
        Def::~Def()
        {
            this->entries_headers_.clear();
            this->ifs_.close();
        }

        /// <summary>Method used to read an entry into the DEF file</summary>
        /// <param name="seqh">The sequence entry header structure to read</param>
        /// <returns>The byte vector containing the sequence</returns>
        const std::vector<byte> ReadEntry(const def_seqh& seqh)
        {   
            std::vector<byte> entry;
            return entry;
        }
    }
}
