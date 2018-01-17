
// qmlosg
#include <GlobeOsg.h>
#include <osgQuickNode.h>

// qt quick stuff
#include<QQuickWindow>
#include<QOpenGLFunctions>

#include <QSGSimpleTextureNode>


// osg
#include <osg/ShapeDrawable>
#include <osg/PolygonMode>
#include <osgViewer/ViewerEventHandlers>
#include <osg/Shape>
#include <osgDB/ReaderWriter>
#include <osgDB/ReadFile>
#include <osgViewer/Viewer>

//osg earth
#include <osgEarthDrivers/gdal/GDALOptions>
#include <osgEarth/Map>
#include <osgEarth/MapNode>
#include <osgEarthUtil/EarthManipulator>
#include <osgEarthDrivers/gdal/GDALOptions>
#include <osgEarth/ImageLayer>

// stl
#include <math.h>
#include <iostream>



class RotateCallback : public osg::NodeCallback
{
public:
    RotateCallback(osg::PositionAttitudeTransform* pat) :
        _pat(pat),
        _angle(0.f)
    {}

    /** Callback method called by the NodeVisitor when visiting a node.*/
    virtual void operator()(osg::Node* node, osg::NodeVisitor* nv)
    {
        osg::Quat rotationX;
        float delta = osg::PI / 6.0 / 60.f; // 30 degrees in a second
        _angle += delta;
        rotationX.makeRotate(_angle, osg::Z_AXIS);
        _pat->setAttitude(rotationX);

        //std::cout << "current rotation is: " << osg::RadiansToDegrees(_angle) << std::endl;

        // note, callback is responsible for scenegraph traversal so
        // they must call traverse(node,nv) to ensure that the
        // scene graph subtree (and associated callbacks) are traversed.
        traverse(node,nv);
    }



protected:
    osg::ref_ptr<osg::PositionAttitudeTransform> _pat;
    float _angle;
};


class PostDrawDebug : public osg::Camera::DrawCallback
{
public:
    /** Functor method called by rendering thread. Users will typically override this method to carry tasks such as screen capture.*/
    virtual void operator () (osg::RenderInfo& renderInfo) const
    {
        std::cout << "YES, I've rendered also frame: " << renderInfo.getState()->getFrameStamp()->getFrameNumber() << std::endl;
    }
};


GlobeOsg::GlobeOsg() :
    _osgQuickNode(NULL),
    _zoomProp(1.0)
{
    setFlag(ItemHasContents);

    // start timer to force a render loop
    startTimer(16);
}


GlobeOsg::~GlobeOsg()
{
}

void GlobeOsg::setZoom(qreal zoom)
{
    _orbit->setDistance(zoom * _defaultDistance);
    _zoomProp = zoom;
}

qreal GlobeOsg::getZoom()
{
    return _zoomProp;
}

void GlobeOsg::setAnimate(bool animate)
{
    if(animate)
    {
        if(_pat->getUpdateCallback() == NULL)
            _pat->setUpdateCallback(new RotateCallback(_pat) );
    }
    else
        _pat->setUpdateCallback(NULL);
}

bool GlobeOsg::getAnimate()
{
    return (_pat->getUpdateCallback() != NULL);
}

QSGNode* GlobeOsg::updatePaintNode(QSGNode* qNode, UpdatePaintNodeData* qNodeData)
{
    //std::cout << "updatePaintNode() " << std::endl;

    if(!_osgQuickNode)
    {
        _osgQuickNode = new OsgQuickNode;
        _osgQuickNode->setQuickWindow(window());

        setupScene();
    }

    // pass events here if any occured
    if(!_mousePressVec.empty())
    {
        int x = _mousePressVec[0].x();
        int y = _mousePressVec[0].y();
        _osgQuickNode->getViewer()->getEventQueue()->mouseButtonPress(x, y, osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON);

        _mousePressVec.clear();
    }

    // pass events here if any occured
    if(!_mouseDragVec.empty())
    {
        int x = _mouseDragVec[0].x();
        int y = _mouseDragVec[0].y();
        _osgQuickNode->getViewer()->getEventQueue()->mouseMotion(x, y);
        std::cout << "Mouse Motion at " << x << ", "  << y << std::endl;
        _mouseDragVec.clear();
    }

    return _osgQuickNode;

}

void GlobeOsg::home()
{
    // reset to home
    std::cout << "func home was called\n";
    _osgQuickNode->getViewer()->home();
}

void GlobeOsg::invisiable()
{
    _osgQuickNode->invisiable();
}

void GlobeOsg::setupScene()
{
    // also add an orbit manipulator
    _orbit = new osgGA::OrbitManipulator;
    _osgQuickNode->getViewer()->setCameraManipulator(_orbit.get());
    _osgQuickNode->getViewer()->home();
    _defaultDistance = _orbit->getDistance();
}

void GlobeOsg::timerEvent(QTimerEvent *)
{
    update();
}


void GlobeOsg::_mousePressEvent(int x, int y)
{
    // fwd mouse event to osg
    _mousePressVec.push_back(osg::Vec2(x, y));
    //_osgViewer->getEventQueue()->mouseButtonPress(x, y, osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON);
}

void GlobeOsg::_mouseDragEvent(int x, int y)
{
    // fwd mouse event to osg
    //std::cout << "MouseDragged at " << x << ", "  << y << std::endl;
    _mouseDragVec.push_back(osg::Vec2(x, y));
    //_osgViewer->getEventQueue()->mouseMotion(x, y);
}
