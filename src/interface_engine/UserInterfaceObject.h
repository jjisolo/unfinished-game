#pragma once

#include <UserInterface.h>

namespace SDL::UI::priv
{
	class UserInterfaceObject
	{
	public:


	protected:
		UIObjectID       m_id;
		UIBinaryState16  m_is_hot;
		UIBinaryState16  m_was_hot;
	};
}