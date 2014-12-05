#include <sstream>
#include <string>
#include <map>
#include <memory>
#include "Types.h"
#include "Path.h"
#include "MemberDataSetter.h"
#include "Conditions/TrueCondition.h"
#include "Assert.h"

template<typename JsonProcessor, typename DataT>
class JsonParser final
{
public:
	using JsonNode = typename JsonProcessor::JsonNode;
	using ErrorT = unsigned int;
	static const ErrorT mDefaultUserError = 0;

	JsonParser() { reset(); }

	template<typename MemberT>
	void bind(std::string path, MemberT DataT::*member, bool mandatory, ErrorT userError = mDefaultUserError)
	{
		using MemberSetter = MemberDataSetter<JsonProcessor, DataT, MemberT>;
	
		Path aPath;
		if(!aPath.init(std::move(path)))
		{
			ASSERT(!"Bad path");
		}
		mLongestPath = std::max(mLongestPath, aPath.getNodes().size());
		
		SetterData aData;
		aData.mSetter.reset(new MemberSetter(member));
		aData.mIsMandatory = mandatory;
		aData.mUserError = std::move(userError);
		mSetters.insert(std::make_pair(std::move(aPath), std::move(aData)));
	}
	
	ResultCode parse(const std::string& json, DataT& data)
	{
		reset();
		
		std::unique_ptr<JsonNode> aNode = JsonProcessor::parse(json, mErrorDescription);
		if(!aNode)
			return ResultCode::parseError;
		return iterate(aNode.get(), data);
	}
	
	const std::string& getErrorDescription() { return mErrorDescription; }
	const ErrorT getUserError() { return mUserError; }
	
private:
	struct SetterData
	{
		SetterData() = default;
		SetterData(SetterData&&) = default;
		SetterData& operator=(SetterData&&) = default;
	
		std::unique_ptr<DataSetter<JsonNode, DataT>> mSetter;
		bool mIsMandatory;
		//Condition mCondition;
		unsigned int mUserError;
	};

	void reset()
	{
		mErrorDescription.clear();
		mUserError = mDefaultUserError;
	}

	void createDescription_(std::stringstream& stream) 
	{}
	template<typename T, typename ... Args>
	void createDescription_(std::stringstream& stream, T&& t, Args&& ... args)
	{
		stream << std::forward<T>(t);
		createDescription(stream, std::forward<Args>(args)...);
	}
	
	template<typename ... Args>
	std::string createDescription(Args&& ... args)
	{
		std::stringstream aStream;
		createDescription(aStream, std::forward<Args>(args)...);
		return aStream.str();
	}
	
	ResultCode iterateByNodes(const std::vector<Path::Node>& pathNodes, 
		unsigned int pathIndex, 
		std::vector<JsonNode*>& nodes)
	{
		ResultCode aCode = ResultCode::ok;

		JsonNode* aNode = nodes.back();
		for(; pathIndex < pathNodes.size(); ++pathIndex)
		{
			auto& aPathNode = pathNodes[pathIndex];
			switch(aPathNode.mType)
			{
			case Path::Type::index:
				aNode = JsonProcessor::getChild(aNode, aPathNode.mData.mIndex, aCode);
				break;
			case Path::Type::name:
				aNode = JsonProcessor::getChild(aNode, aPathNode.mData.mName, aCode);
				break;
			};
			
			if(aNode == nullptr)
				break;
			
			nodes.push_back(aNode);
		}
		return aCode;
	}
	
	unsigned int getDiffIndex(const std::vector<Path::Node>& nodes1, const std::vector<Path::Node>& nodes2)
	{
		unsigned int i = 0;
		unsigned int aMinSize = std::min(nodes1.size(), nodes2.size());
		for(; (i < aMinSize) && (nodes1[i] == nodes2[i]); ++i);
		return i;
	}
	
	ResultCode setData(JsonNode* node, const SetterData& data, DataT& userData)
	{
		return data.mSetter->setData(node, userData) ? ResultCode::ok : ResultCode::wrongType;
	}
	
	ResultCode iterate(JsonNode* root, DataT& userData)
	{
		static const std::vector<Path::Node> aEmptyNodes;

		ResultCode aCode = ResultCode::ok;
		std::vector<JsonNode*> aJsonNodes;
		aJsonNodes.reserve(mLongestPath);
		aJsonNodes.push_back(root);
		
		const std::vector<Path::Node>* aPrevNodes = &aEmptyNodes;
		for(auto it = mSetters.begin(); it != mSetters.end(); ++it)
		{
			auto& aPathNodes = it->first.getNodes();
			unsigned int aPathIndex = getDiffIndex(*aPrevNodes, aPathNodes);
			if(aPathIndex < aJsonNodes.size())
			{
				aJsonNodes.resize(aPathIndex+1); // TODO: check on memory changes;

				aCode = iterateByNodes(aPathNodes, aPathIndex, aJsonNodes);
				if(aCode == ResultCode::ok)
				{
					aCode = setData(aJsonNodes.back(), it->second, userData);
				}
			}
			
			if((aCode != ResultCode::ok) && it->second.mIsMandatory)
			{
				mErrorDescription = "Good error description will be generated later";
				mUserError = it->second.mUserError;
				break;
			}
			aPrevNodes = &aPathNodes;
		}
		return aCode;
	};

	std::multimap<Path, SetterData> mSetters;
	unsigned int mLongestPath = 0;
	
	std::string mErrorDescription;
	ErrorT mUserError = mDefaultUserError;
};
