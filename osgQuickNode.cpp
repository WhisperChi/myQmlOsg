
// qmlosg
#include <osgQuickNode.h>

// qt
#include <QtCore/QCoreApplication>
#include <QtCore/QDir>
#include <QSurfaceFormat>
#include <QOpenGLContext>
#include <QOpenGLFunctions>

// osg
#include <osg/Texture2D>
#include <osg/ShapeDrawable>
#include <osg/PolygonMode>
#include <osg/Group>
#include <osgGA/OrbitManipulator>
#include <osgViewer/ViewerEventHandlers>
#include <osg/NodeCallback>
#include <osgDB/ReaderWriter>
#include <osgDB/ReadFile>

#include <osgEarth/MapNode>

// stl
#include <iostream>


class RotateLocalSceneCallback : public osg::NodeCallback
{
public:
    RotateLocalSceneCallback(osg::PositionAttitudeTransform* pat) :
        _pat(pat),
        _angle(0.f)
    {}

    /** Callback method called by the NodeVisitor when visiting a node.*/
    virtual void operator()(osg::Node* node, osg::NodeVisitor* nv)
    {
        osg::Quat rotationX;
        float delta = osg::PI / 12.0 / 60.f; // 30 degrees in a second
        _angle += delta;
        rotationX.makeRotate(_angle, osg::Z_AXIS);
        _pat->setAttitude(rotationX);

        //std::cout << "local scene rotation is: " << osg::RadiansToDegrees(_angle) << std::endl;

        // note, callback is responsible for scenegraph traversal so
        // they must call traverse(node,nv) to ensure that the
        // scene graph subtree (and associated callbacks) are traversed.
        traverse(node,nv);
    }

protected:
    osg::ref_ptr<osg::PositionAttitudeTransform> _pat;
    float _angle;
};


OsgQuickNode::OsgQuickNode()
    : QSGGeometryNode()
    , _qGeometry(QSGGeometry::defaultAttributes_TexturedPoint2D(), 4)
    , _qTexture(0)
    , _samples(0)
    , _quickWindow(0)
    , _osgContext(0)
    , _qtContext(0)
    , _AAEnabled(false)
    , _initialized(false)
    , _dirtyFBO(false)
{
    setMaterial(&_qTexMaterial);
    setOpaqueMaterial(&_qOpaqueMaterial);
    setGeometry(&_qGeometry);

    setFlag(UsePreprocess);
}

OsgQuickNode::~OsgQuickNode()
{
    // any osg stuff to cleanup here??

}


void OsgQuickNode::saveOsgState()
{
    _osgContext->doneCurrent();
    _qtContext->makeCurrent(_quickWindow);
}

void OsgQuickNode::restoreOsgState()
{
    _qtContext = QOpenGLContext::currentContext();
    _qtContext->doneCurrent();

    _osgContext->makeCurrent(_quickWindow);
}



void OsgQuickNode::preprocess()
{
    //std::cout << "preprocess() " << std::endl;

    renderOsgFrame();
}

void OsgQuickNode::setQuickWindow(QQuickWindow *window)
{
    _quickWindow = window;

    // create a new shared OpenGL context to be used exclusively by OSG
    _osgContext = new QOpenGLContext();
    _osgContext->setFormat(_quickWindow->requestedFormat());
    _osgContext->setShareContext(QOpenGLContext::currentContext());
    _osgContext->create();

    // ok, now init the viewer as embedded, with render target to FBO
    init();
}

void OsgQuickNode::renderOsgFrame()
{
    // restore the osg gl context
    restoreOsgState();

    if(!_osgViewer->isRealized())
    {
        OSG_ALWAYS << "osgViewer REALIZE called!" << std::endl;
        _osgViewer->realize();
    }


    if (_dirtyFBO)
    {
        updateFBO();
        _dirtyFBO = false;
    }

    //std::cout << "viewer->frame()" << std::endl;
    _osgViewer->frame();

    glFlush();

    // if just inited copy the FBO texture to the Quick texture
    if(!_initialized)
    {
        QSGGeometry::updateTexturedRectGeometry(&_qGeometry,
                                                QRectF(0, 0, _quickWindow->width(), _quickWindow->height()),
                                                // invert uv not to get an y flipped result
                                                QRectF(0, 1, 1, -1)); // normally this would be 0, 0, 1, 1

        _qTexture = _quickWindow->createTextureFromId(getGLtexId(), _quickWindow->size());

        _qTexMaterial.setTexture(_qTexture);
        _qOpaqueMaterial.setTexture(_qTexture);

        _initialized = true;
    }

    // we're done with the osg state, restore the Qt one
    saveOsgState();
}

void OsgQuickNode::updateFBO()
{
    // TODO update the FBO after resize


    // QSGGeometry::updateTexturedRectGeometry();

    //delete m_texture;
    //m_texture = m_quickWindow->createTextureFromId(nativeTexture->getGLID(), m_size);

    //m_material.setTexture(m_texture);
    //m_materialO.setTexture(m_texture);
}

void OsgQuickNode::setSize(const QSize &size)
{
    if (size == m_size)
        return;

    m_size = size;
    _dirtyFBO = true;
    markDirty(DirtyGeometry);
}

void OsgQuickNode::setAAEnabled(bool enable)
{
    if (_AAEnabled == enable)
        return;

    _AAEnabled = enable;
    _dirtyFBO = true;
    markDirty(DirtyMaterial);
}

void OsgQuickNode::init()
{
    const QOpenGLContext *ctx = QOpenGLContext::currentContext();
    QSurfaceFormat format = ctx->format();
    _samples = format.samples();

    int viewWidth = _quickWindow->width();
    int viewHeight = _quickWindow->height();


    // create the view of the scene.
    _osgViewer = new osgViewer::Viewer;

    // setup as embedded
    osgViewer::GraphicsWindowEmbedded* embeddedWin = _osgViewer->setUpViewerAsEmbeddedInWindow(0, 0, viewWidth, viewHeight);

    OSG_ALWAYS << "----> SETTING UP osg Viewer with viewport " << viewWidth << ", " << viewHeight << std::endl;


    // tell the viewer to render on an FBO
    osg::Camera* pCamera = _osgViewer->getCamera();
    pCamera->setViewport(0, 0, viewWidth, viewHeight);
    pCamera->setRenderTargetImplementation(osg::Camera::FRAME_BUFFER_OBJECT);

    // set a grey background color
    pCamera->setClearColor(osg::Vec4(0.7, 0.7, 0.7, 1.0));

    // attach a texture
    osg::Texture2D* texture2D = new osg::Texture2D;
    texture2D->setTextureSize(viewWidth, viewHeight);
    texture2D->setInternalFormat(GL_RGBA);
    texture2D->setFilter(osg::Texture2D::MIN_FILTER,osg::Texture2D::LINEAR);
    texture2D->setFilter(osg::Texture2D::MAG_FILTER,osg::Texture2D::LINEAR);

    // attach the texture and use it as the color buffer.
    pCamera->attach(osg::Camera::COLOR_BUFFER, texture2D,
                    0, 0, false,
                    _samples, _samples);

    osg::ref_ptr<osgEarth::MapNode> mapNode = new osgEarth::MapNode;
    // save a reference to the texture object bound to the fbo
    _fboTex = texture2D;


#if 1
    // create a sample scene
    osg::Group* sceneRoot = new osg::Group;

    sceneRoot->addChild(mapNode);

    //sceneRoot->addChild(createScene());
    _osgViewer->setSceneData(sceneRoot);
#endif

}

GLuint OsgQuickNode::getGLtexId()
{
    if(_fboTex.valid())
    {
        unsigned int ctxId = _osgViewer->getCamera()->getGraphicsContext()->getState()->getContextID();
        return _fboTex->getTextureObject(ctxId)->id();
    }

    return 0;

}


osg::Node* OsgQuickNode::createScene()
{
    // setup a sphere to render
    osg::ShapeDrawable* pSD = new osg::ShapeDrawable(new osg::Sphere(osg::Vec3(0, 0, 0), 5));
    pSD->setColor(osg::Vec4(1, 0, 0, 1.0));
    osg::Geode* geode = new osg::Geode;
    geode->addDrawable(pSD);

    geode->getOrCreateStateSet()->setAttributeAndModes(new osg::PolygonMode(osg::PolygonMode::FRONT_AND_BACK, osg::PolygonMode::LINE));
    //geode->getOrCreateStateSet()->setMode(GL_BLEND, osg::StateAttribute::ON);

    osg::Shader* pvert = new osg::Shader(osg::Shader::VERTEX);
    // read vertex shader from resources
    {
        QFile file(":/shaders/globe.vert");
        file.open(QIODevice::ReadOnly | QIODevice::Text);

        QTextStream in(&file);
        QString line = in.readAll();
        pvert->setShaderSource(line.toStdString());
    }

    osg::Shader* pFrag = new osg::Shader(osg::Shader::FRAGMENT);
    // read frag shader from resources
    {
        QFile file(":/shaders/globe.frag");
        file.open(QIODevice::ReadOnly | QIODevice::Text);

        QTextStream in(&file);
        QString line = in.readAll();
        pFrag->setShaderSource(line.toStdString());
    }

    osg::Program* pProg = new osg::Program;
    pProg->addShader(pvert);
    pProg->addShader(pFrag);
    geode->getOrCreateStateSet()->setAttributeAndModes(pProg);

    _pat = new osg::PositionAttitudeTransform;
    _pat->setPosition(osg::Vec3(30, 0, 10));
    //_pat->addChild(geode);

    _pat->addUpdateCallback(new RotateLocalSceneCallback(_pat));

    return _pat.get();

}
