#include <OgreOverlayPrerequisites.h>
#include <OgreOverlayManager.h>
#include <OgreOverlayContainer.h>
#include <OgreOverlay.h>

#include "Overlay.hpp"

Overlay::Overlay()
{
    Ogre::OverlayManager& overlayManager = Ogre::OverlayManager::getSingleton();
    // Create an overlay
    Ogre::Overlay* overlay = overlayManager.create( "OverlayName" );

    // Create a panel
    Ogre::OverlayContainer* panel = static_cast<Ogre::OverlayContainer*>( overlayManager.createOverlayElement( "Panel", "PanelName" ) );
    panel->setPosition( 0.0, 0.0 );
    panel->setDimensions( 0.1, 0.1 );
    panel->setMaterialName( "BaseWhite" );
    // Add the panel to the overlay
    overlay->add2D( panel );

    // Show the overlay
    overlay->show();
}
