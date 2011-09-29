#include "Graphics/Texture.h"
#include "dblox/ResourceManager.h"

namespace Resource
{
    template class Resource::Manager<Graphics::Texture*>;

	template <class T>
	Manager<T>::Manager( T (*CreateFunc)(const std::string&), void (*DestroyFunc)(T) )
		: m_pfnCreateFunc(CreateFunc)
		, m_pfnDestroyFunc(DestroyFunc)
	{

	}

	template <class T>
	Manager<T>::~Manager()
	{
		for( std::map<unsigned int, InternalResource_t>::iterator iter = m_Resources.begin(); iter != m_Resources.end(); ++iter )
		{
			m_pfnDestroyFunc(iter->second.Resource);
			m_ResourceNames.erase(iter->second.Filename);
		}

		m_Resources.clear();
	}

	template <class T>
	Resource::Handle<T>* Manager<T>::Load( const std::string& Filename )
	{
		std::map<std::string, unsigned int>::const_iterator iter = m_ResourceNames.find(Filename);
			
		if( iter != m_ResourceNames.end() )
			return new Handle<T>(this, iter->second);

		InternalResource_t InternalResource = { 0, 0 };
		unsigned int ResID;
			
		if( m_UnusedIDs.size() > 0 )
		{
			ResID = m_UnusedIDs.top();
			m_UnusedIDs.pop();
		}
		else
		{
			ResID = m_Resources.size() + 1;
		}

		InternalResource.Resource = m_pfnCreateFunc(Filename);
		m_Resources.insert(std::make_pair(ResID, InternalResource));
		m_ResourceNames.insert(std::make_pair(Filename, ResID));

		return new Handle<T>(this, ResID);
	}

	template <class T>
	void Manager<T>::Reference( unsigned int ResID )
	{
		m_Resources[ResID].References++;
	}

	template <class T>
	void Manager<T>::Dereference( unsigned int ResID )
	{
		m_Resources[ResID].References--;

		if( m_Resources[ResID].References <= 0 )
		{
			m_pfnDestroyFunc(m_Resources[ResID].Resource);
			m_UnusedIDs.push(ResID);
			m_ResourceNames.erase(m_Resources[ResID].Filename);
			m_Resources.erase(ResID);
		}
	}
}