#pragma once

template<typename Type>
class TAnyValue
{
public:
	Type type;
	void* data;

public:
	struct ValueAccesser
	{
		virtual void Alloc(TAnyValue& value) = 0;
		virtual void Assign(TAnyValue& value, const void* from) = 0;
		virtual void Free(TAnyValue& value) = 0;
		virtual size_t GetSize() = 0;
	};

	struct TypeNotMatchErroy : std::exception
	{
		virtual char const* what() const
		{
			return "type does not match!";
		}
	};

public:
	TAnyValue()
		: type(eNull)
		, data(nullptr)
	{
	}

	TAnyValue(Type type)
		: type(type)
		, data(nullptr)
	{
		ValueAccessers[this->type]->Alloc(*this);
	}

	TAnyValue(const TAnyValue& from)
	{
		CopyFrom(from);
	}

	template<typename T>
	TAnyValue(const T& from)
		: type(eNull)
		, data(nullptr)
	{
		CopyFrom(TypeTraits<T>::GetType(), &from);
	}

	~TAnyValue()
	{
		if (data)
		{
			ValueAccessers[this->type]->Free(*this);
		}
	}

	TAnyValue& operator=(const TAnyValue& from)
	{
		CopyFrom(from);
		return *this;
	}

	void CopyFrom(const TAnyValue& from)
	{
		CopyFrom(from.type, from.data);
	}

	Type GetType() const
	{
		return type;
	}

	size_t GetSize() const
	{
		return ValueAccessers[this->type]->GetSize();
	}

	void* GetData() const
	{
		return data;
	}

	template<typename T>
	const T* GetPtr() const
	{
		if (TypeTraits<T>::GetType() != type)
		{
			throw TypeNotMatchErroy();
			return nullptr;
		}
		return (T*)data;
	}

	template<typename T>
	T Get() const
	{
		if (TypeTraits<T>::GetType() != type)
		{
			throw TypeNotMatchErroy();
			return T();
		}
		return *(T*)data;
	}

	void CopyFrom(Type type, const void* data)
	{
		if (this->type == eNull)
		{
			if (this->data)
			{
				ValueAccessers[this->type]->Free(*this);
			}
			this->type = type;
			ValueAccessers[this->type]->Alloc(*this);
		}
		else if (this->type != type)
		{
			throw TypeNotMatchErroy();
			return;
		}

		ValueAccessers[type]->Assign(*this, data);
	}
};
