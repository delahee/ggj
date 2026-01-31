#pragma once
#include "r2/Node.hpp"
#include "rui/Dock.hpp"
#include "rs/Display.hpp"

namespace rui {
	/************************************************************************/
	// A container that allows docking 
	// always measure refSize
	/************************************************************************/
	class Canvas : public r2::Node, public rui::IContainer {
		typedef r2::Node Super;
	public:
		bool									enabled = true;
		rd::Sig									sigOnResize;
		std::unordered_map<r2::Node*, rd::SignalHandler*> 
												resizeHolder;
		Vector2i								fallbackRefSize = Vector2i(1920, 1080);

												Canvas(r2::Node* parent = 0);
		virtual									~Canvas();
		virtual void							onResize(const Vector2& ns) override;
		virtual void							resetContent();
		virtual void							dispose() override;

		float									getPixelRatio()const;
		//interface impls
		virtual Vector2							getRefSize();//returns authoring size of container
		virtual Vector2							getWorkingSize();//returns size you should work with
		virtual float							getScaleX();
		virtual float							getScaleY();
		virtual Vector2							getActualSize();//returns current size for docking in virtual units for ex 1920x1080 if in 4k will return 1920x1080 but parent is likely scale x2

		virtual void							onRemoveChild(r2::Node* n);

		//helpers for theoritically docking measures, don't use width() as it is content based
		inline float							w() { return getActualSize().x; };
		inline float							h() { return fallbackRefSize.y; };
		
		bool									isDocked(r2::Node* spr);

		r2::Node*								dock(r2::Node* spr, rd::Dir dir, float ofs = 0);
		r2::Node*								dock(r2::Node* spr, rd::Dir dir, Vector2 ofs);
		void									redock(r2::Node* spr);

		//don't suscribe to heavy resize for quick patching

		/**
		* Docking will not take internal pivot into account,
		* it will make its best to respect docking propertis for a top-left based sprite based on width and height properties
		* which means for a text, you better not use blockAlign ( our model is not very good since 'block align' and 'position align' should be separated )
		*/
		r2::Node*								dockOnce(r2::Node* spr, rd::Dir dir, Vector2 ofs);


		/**
		* Docking will not take internal pivot into account,
		* it will make its best to respect docking propertis for a top-left based sprite based on width and height properties
		* which means for a text, you better not use blockAlign ( our model is not very good since 'block align' and 'position align' should be separated )
		*/
		r2::Node*								dockUp(r2::Node* spr, float ofs = 0);

		/**
		* Docking will not take internal pivot into account,
		* it will make its best to respect docking propertis for a top-left based sprite based on width and height properties
		* which means for a text, you better not use blockAlign ( our model is not very good since 'block align' and 'position align' should be separated )
		*/
		r2::Node*								dockLeft(r2::Node* spr, float ofs = 0);

		void									undock(r2::Node* spr);

		virtual void							runAndRegResize(std::function<void(void)>);
		vec2									getCenter();

		void									dumpDocked();
		r::Vector2								getMousePos();
		virtual void							im()override;
		static bool								DEBUG;

		virtual	NodeType						getType() const override;;
		virtual void							serialize(Pasta::JReflect& f, const char* name = nullptr) override;

	};
}