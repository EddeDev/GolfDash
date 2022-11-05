#pragma once

#include "PrimitiveTypes.h"

#include <atomic>

namespace gd {

	class ReferenceCounted
	{
	public:
		void IncrementReferenceCount() const { m_ReferenceCount++; }
		void DecrementReferenceCount() const { m_ReferenceCount--; }

		uint32 GetReferenceCount() const { return m_ReferenceCount.load(); }
	private:
		mutable std::atomic<uint32> m_ReferenceCount = 0;
	};

	template<typename T>
	class Ref
	{
	public:
		Ref()
			: m_Reference(nullptr)
		{
		}

		Ref(std::nullptr_t)
			: m_Reference(nullptr)
		{
		}

		Ref(T* instance)
			: m_Reference(instance)
		{
			IncrementReferenceCount();
		}

		Ref(const Ref<T>& other)
			: m_Reference(other.m_Reference)
		{
			IncrementReferenceCount();
		}

		template<typename TOther>
		Ref(const Ref<TOther>& other)
		{
			m_Reference = static_cast<T*>(other.m_Reference);
			IncrementReferenceCount();
		}

		template<typename TOther>
		Ref(Ref<TOther>&& other)
		{
			m_Reference = static_cast<T*>(other.m_Reference);
			other.m_Reference = nullptr;
		}

		~Ref()
		{
			DecrementReferenceCount();
		}

		Ref& operator=(std::nullptr_t)
		{
			DecrementReferenceCount();
			m_Reference = nullptr;
			return *this;
		}

		Ref& operator=(const Ref<T>& other)
		{
			other.IncrementReferenceCount();
			DecrementReferenceCount();
			m_Reference = other.m_Reference;
			return *this;
		}

		template<typename TOther>
		Ref& operator=(const Ref<TOther>& other)
		{
			DecrementReferenceCount();
			m_Reference = other.m_Reference;
			other.m_Reference = nullptr;
			return *this;
		}

		operator bool() { return m_Reference != nullptr; }
		operator bool() const { return m_Reference != nullptr; }

		T* operator->() { return m_Reference; }
		const T* operator->() const { return m_Reference; }

		T& operator*() { return *m_Reference; }
		const T& operator*() const { return *m_Reference; }

		T* Get() { return m_Reference; }
		const T* Get() const { return m_Reference; }

		template<typename TOther>
		Ref<TOther> As() const
		{
			return Ref<TOther>(*this);
		}

		template<typename... Args>
		static Ref<T> Create(Args&&... args)
		{
			return Ref<T>(new T(std::forward<Args>(args)...));
		}
	private:
		void IncrementReferenceCount() const
		{
			if (m_Reference)
				m_Reference->IncrementReferenceCount();
		}

		void DecrementReferenceCount() const
		{
			if (!m_Reference)
				return;

			m_Reference->DecrementReferenceCount();

			if (m_Reference->GetReferenceCount() == 0)
			{
				delete m_Reference;
				m_Reference = nullptr;
			}
		}
	private:
		mutable T* m_Reference;

		template<class TOther>
		friend class Ref;
	};

}