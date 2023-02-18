#pragma once

#include <UserInterface.h>

namespace SDL::UI::priv
{
	class UserInterfaceObject
	{
	public:

		UIObjectID get_id();

		void  set_id(UIObjectID id);

		UIBinaryState16 get_hot();

		void  set_hot(UIBinaryState16 id);

		UIBinaryState16 get_id();

		void  set_id(UIBinaryState16 id);

	protected:
		UIObjectID       m_id;
		UIBinaryState16  m_is_hot;
		UIBinaryState16  m_was_hot;
	};
}