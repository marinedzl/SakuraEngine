#pragma once

enum RenderStateType
{
	RS_BlendState,
	RS_RasterizerState,
	RS_DepthStencilState,
	RS_Count,
};

template<typename _Res >
class RenderState
{
public:
	virtual void Release();
	typedef int _Key;
	static RenderStateType GetType();
	_Res* GetRes(ID3D11Device* device, _Key& key);
};

template<>
class RenderState < ID3D11BlendState > : public Object
{
public:
	virtual void Release()
	{
		ReleaseMap(mRes); 
		delete this;
	}
	typedef D3D11_BLEND_DESC _Key;
	static RenderStateType GetType() { return RS_BlendState; }
	ID3D11BlendState* GetRes(ID3D11Device* device, const D3D11_BLEND_DESC& key)
	{
		ID3D11BlendState* ret = nullptr;
		ResMap::iterator iter = mRes.find(key);
		if (iter != mRes.end())
		{
			ret = iter->second;
		}
		else
		{
			device->CreateBlendState(&key, &ret);
			if (ret)
				mRes.insert(std::make_pair(key, ret));
		}
		return ret;
	}
private:
	struct Compare
	{
		bool operator()(const D3D11_BLEND_DESC& left, const D3D11_BLEND_DESC& right) const
		{
			if (left.RenderTarget[0].BlendEnable < right.RenderTarget[0].BlendEnable) return true;
			else if (left.RenderTarget[0].BlendOp < right.RenderTarget[0].BlendOp) return true;
			else if (left.RenderTarget[0].SrcBlend < right.RenderTarget[0].SrcBlend) return true;
			else if (left.RenderTarget[0].DestBlend < right.RenderTarget[0].DestBlend) return true;
			return false;
		}
	};
	typedef std::map<D3D11_BLEND_DESC, ID3D11BlendState*, Compare> ResMap;
private:
	ResMap mRes;
};

template<>
class RenderState < ID3D11RasterizerState > : public Object
{
public:
	virtual void Release()
	{
		ReleaseMap(mRes);
		delete this;
	}
	typedef D3D11_RASTERIZER_DESC _Key;
	static RenderStateType GetType() { return RS_RasterizerState; }
	ID3D11RasterizerState* GetRes(ID3D11Device* device, const D3D11_RASTERIZER_DESC& key)
	{
		ID3D11RasterizerState* ret = nullptr;
		ResMap::iterator iter = mRes.find(key);
		if (iter != mRes.end())
		{
			ret = iter->second;
		}
		else
		{
			device->CreateRasterizerState(&key, &ret);
			if (ret)
				mRes.insert(std::make_pair(key, ret));
		}
		return ret;
	}
private:
	struct Compare
	{
		bool operator()(const D3D11_RASTERIZER_DESC& left, const D3D11_RASTERIZER_DESC& right) const
		{
			if (left.FillMode < right.FillMode) return true;
			else if (left.CullMode < right.CullMode) return true;
			else if (left.FrontCounterClockwise < right.FrontCounterClockwise) return true;
			return false;
		}
	};
	typedef std::map<D3D11_RASTERIZER_DESC, ID3D11RasterizerState*, Compare> ResMap;
private:
	ResMap mRes;
};

template<>
class RenderState < ID3D11DepthStencilState > : public Object
{
public:
	virtual void Release()
	{
		ReleaseMap(mRes);
		delete this;
	}
	typedef D3D11_DEPTH_STENCIL_DESC _Key;
	static RenderStateType GetType() { return RS_DepthStencilState; }
	ID3D11DepthStencilState* GetRes(ID3D11Device* device, const D3D11_DEPTH_STENCIL_DESC& key)
	{
		ID3D11DepthStencilState* ret = nullptr;
		ResMap::iterator iter = mRes.find(key);
		if (iter != mRes.end())
		{
			ret = iter->second;
		}
		else
		{
			device->CreateDepthStencilState(&key, &ret);
			if (ret)
				mRes.insert(std::make_pair(key, ret));
		}
		return ret;
	}
private:
	struct Compare
	{
		bool operator()(const D3D11_DEPTH_STENCIL_DESC left, const D3D11_DEPTH_STENCIL_DESC& right) const
		{
			if (left.DepthEnable < right.DepthEnable) return true;
			else if (left.DepthWriteMask < right.DepthWriteMask) return true;
			else if (left.DepthFunc < right.DepthFunc) return true;
			return false;
		}
	};
	typedef std::map<D3D11_DEPTH_STENCIL_DESC, ID3D11DepthStencilState*, Compare> ResMap;
private:
	ResMap mRes;
};

class RenderStateProxy
{
public:
	template<typename T>
	RenderStateProxy(T* ptr) : mType(T::GetType()), mPtr(ptr) {}
	virtual ~RenderStateProxy() { SAFE_RELEASE(mPtr); }
	template<typename T>
	T* GetPtr() { return T::GetType() == mType ? static_cast<T*>(mPtr) : nullptr; }
private:
	RenderStateType mType;
	Object* mPtr;
};

class RenderStateManager
{
public:
	RenderStateManager();
	virtual ~RenderStateManager();
	void Release();
	bool Init();
	template<typename _Res>
	_Res* GetRenderState(const typename RenderState<_Res>::_Key& key)
	{
		_Res* ret = nullptr;
		RenderState<_Res>* rs = mRenderStateProxy[RenderState<_Res>::GetType()]->GetPtr<RenderState<_Res> >();
		CHECK(rs);
		ret = rs->GetRes(gCore.GetDevice(), key);
	Exit0:
		return ret;
	}
	ID3D11RasterizerState* GetRasterizerState(D3D11_CULL_MODE cull, D3D11_FILL_MODE fill);
private:
	RenderStateProxy* mRenderStateProxy[RS_Count];
};

extern RenderStateManager gRenderStateManager;
