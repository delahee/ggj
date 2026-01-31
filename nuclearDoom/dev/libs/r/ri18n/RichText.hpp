#pragma once

#include "r2/Text.hpp"
#include "rd/Style.hpp"

namespace Pasta { struct JReflect; }

namespace ri18n {
	struct RichTextConf {
		bool mapCharactersToAst = false;
	};
	struct ActionDisplayInfo{
		//inputs
		int			fontSize = 24;
		int			playerId = 0;

		//outputs
		int			ofsX = 0;
		int			ofsY = 0;
		r2::Node*	raw=0;
	};
	/**
	 * rich texts are slow avoid them in intense gameplay...
	 * here is a formatting example
	 * *word* <- emphasize
	 * **word** <- strong
	 * <font color='#mycols'></font>
	 * <color val='#mycols'></color>
	 * see ActionDisplayInfo to display action buttons
	 */
	class RichText : public r2::Text {
		private:
			typedef r2::Text Super;
		public:
			Str									srcTree;
			AstNode*							tree = 0;//alter the tree at you own risk

			//fires all these events when display occurs. you may want to run these through a proper interpreter if need be
			std::function<void(void)>			onEmStart;
			std::function<void(void)>			onEmEnd;

			std::function<void(void)>			onStrongStart;
			std::function<void(void)>			onStrongEnd;

			std::function<void(void)>			onItalicStart;
			std::function<void(void)>			onItalicEnd;

			std::function<void(const char*)>	onEvent;
			std::function<void(const char*)>	onCondEvent;
			std::function<void(const char*)>	onCondUniqueEvent;
			std::function<void(const char*)>	onUniqueEvent;
			std::function<void(const char*)>	onImportantEvent;

			std::function<void(const char*,AstNode*)>	onTagStart;
			std::function<void(const char*)>			onTagEnd;

			std::function<void(const char*)>	onTagFrom;

			std::function<void(void)>			onFirstChar;
			std::function<void(void)>			onLastChar;
			std::function<void(const char*)>	onScript;

			eastl::vector<eastl::vector<AstNode*>>
				charsToNodeStarts;
			eastl::vector<eastl::vector<AstNode*>>
				charsToNodeEnd;

			rd::Style					style;
			std::vector<Str>			classes;
			r::Color					emColor = r::Color::Orange;
			r::Color					strongColor = r::Color::Red;
			r::opt<r::Color>			strongOutlineColor;
			rd::AgentList				al;
			
			static RichText*			mk(const char * txt, r2::Node* parent = nullptr);
			static RichText*			mk(const char * txt, RichTextConf conf, r2::Node* parent );

			inline static RichText*		mk(const std::string& txt, r2::Node* parent = nullptr)	{ return mk(txt.c_str(), parent); };
			inline static RichText*		mk(const Str& txt, r2::Node* parent = nullptr)			{ return mk(txt.c_str(), parent); };

										RichText(AstNode* _tree, r2::Node* parent);

										RichText(rd::Font* fnt, const std::string& t, r2::Node* parent = nullptr);
										RichText(rd::Font* fnt, const Str& t, r2::Node* parent = nullptr);
										RichText(rd::Font* fnt = nullptr, const char* t = nullptr, r2::Node* parent = nullptr);
			virtual						~RichText();

			void						resetCallbacks();
			virtual void				destroy() override;
			virtual void				reset() override;
			virtual	void				dispose() override;
			virtual	NodeType			getType() const override { return NodeType::NT_RICH_TEXT; };

			virtual const char*			setText(const char * str) override;
			void						setStyle(rd::Style& st);

			void						setTree(AstNode * tree );
			void						setTree(const char * tree);

			virtual void				cache() override;
			void						renderNode(AstNode* tree, float* x, float* y);

			void						execute(std::function<void(AstNode*)> f);

			void						stdTagStart(const char * t, AstNode * node);
			void						stdTagEnd(const char * t);

			r::Color					getTextColor(int idx = -1);
			virtual void				setTextColor(const r::Color &c) override;

			static r::Vector2			getTextExtent(rd::Font* fnt, int fontSize, const char* txt);
			static r::Vector2			getTextSimulatedSize(rd::Font* fnt, int fontSize, const char* txt);

			void						im() override;
			virtual void				serialize(Pasta::JReflect& jr, const char * name = nullptr) override;

			void						updateStyle();
			virtual void				applyStyle(const rd::Style& st) override;

			virtual void				update(double dt) override;

			static std::function<bool(const char *,ActionDisplayInfo&)> 
										getActionDisplayInfo;

			virtual void				toPool() override;
			static ri18n::RichText*		fromPool(rd::Font* fnt, const char* txt, r::Color c = {}, Node* parent = nullptr);
			static ri18n::RichText*		fromPool(rd::Font* fnt, const char* txt, Node* parent = nullptr);
		protected:
			RichTextConf				conf;

			void						pushCurrentColor();
			void						popCurrentColor();

			void						pushItalic();
			void						popItalic();

			void						pushCurrentOutline();
			void						popCurrentOutline();
			
			void						ensureCharMaps();
			void						addInlinedNode(r2::Node* n);
			void						addAction(ActionDisplayInfo& info);
			
			std::vector<rd::Font*>		fonts;
			std::vector<int>			textSize;

			float						curX = 0;
			float						curY = 0;

			bool						markItalicCharacters = false;
			rd::BitArray*				italicMask = 0;

			bool						autoItalic = false;
	};
}
