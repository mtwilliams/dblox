#ifndef DBLOX_RESOURCE_MANAGER_H
#define DBLOX_RESOURCE_MANAGER_H

#include <map>
#include <stack>

namespace Resource
{
	template <class T>
	class Manager;

	template <class T>
	class Handle
	{
		public:
			~Handle()
			{
				m_pResManager->Dereference(m_ResID);
			}

			void Dispose( void )
			{
				delete this;
			}

			operator T() const
			{
				return m_pResManager->m_Resources[m_ResID].Resource;
			}

		protected:
			unsigned int m_ResID;
			Manager<T>* m_pResManager;

		protected:
			Handle( Manager<T>* ResManager, unsigned int ResID )
				: m_pResManager(ResManager)
				, m_ResID(ResID)
			{
				m_pResManager->Reference(ResID);
			}

		friend Manager<T>;
	};

	template <class T>
	class Manager
	{
		protected:
			typedef struct
			{
				T Resource;
				unsigned int References;
				std::string Filename;
			} InternalResource_t;

		public:
			Manager( T (*CreateFunc)(const std::string&), void (*DestroyFunc)(T) );
			~Manager();

			Resource::Handle<T>* Load( const std::string& Filename );

		protected:
			std::stack<unsigned int> m_UnusedIDs;
			std::map<unsigned int, InternalResource_t> m_Resources;
			std::map<std::string, unsigned int> m_ResourceNames;
			T (*m_pfnCreateFunc)(const std::string&);
			void (*m_pfnDestroyFunc)(T);

			void Reference( unsigned int ResID );
			void Dereference( unsigned int ResID );

		friend Handle<T>;
	};
}

#endif