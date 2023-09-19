#include "QTTEST.h"
#include <QtWidgets/QApplication>
#include <osgQt/GraphicsWindowQt>
#include <osgViewer/Viewer>
#include <osg/Geode>
#include <osg/Node>
#include <osg/Group>
#include <osgGA/StateSetManipulator>
#include <osgGA/TrackballManipulator>
#include <osgDB/ReadFile>
#include <osg/Material>
#include <QGridLayout>
#include <QGLWidget>
#include <BRepPrimAPI_MakeBox.hxx>
#include <TopoDS_Shape.hxx>
#include <BRepMesh_DiscretRoot.hxx>
#include <BRepMesh_DiscretFactory.hxx>
#include <TopExp_Explorer.hxx>
#include <TopoDS_Face.hxx>
#include <TopoDS.hxx>
#include <Poly.hxx>
#include <BRepTools.hxx>
#include <stdio.h>

osg::ref_ptr<osg::Geode> BuildShape(TopoDS_Shape occShape)
{
	osg::ref_ptr<osg::Geode> geode = new osg::Geode();
	osg::ref_ptr<osg::Geometry> triGeom = new osg::Geometry();
	osg::ref_ptr<osg::Vec3Array> triVertices = new osg::Vec3Array();
	osg::ref_ptr<osg::Vec3Array> triNormals = new osg::Vec3Array();
	osg::ref_ptr<osg::Vec2Array> triUvs = new osg::Vec2Array();
	osg::ref_ptr<osg::DrawElementsUInt> triIndexs = new osg::DrawElementsUInt(osg::PrimitiveSet::TRIANGLES);

	int newStart = 0;
	Handle(BRepMesh_DiscretRoot) aMeshAlgo = BRepMesh_DiscretFactory::Get().Discret(occShape, 0.1, 0.5);
	if (!aMeshAlgo.IsNull())
	{
		aMeshAlgo->Perform();
	}

	for (TopExp_Explorer aFaceExplorer(occShape, TopAbs_FACE); aFaceExplorer.More(); aFaceExplorer.Next())
	{
		TopoDS_Face aFace = TopoDS::Face(aFaceExplorer.Current());
		TopLoc_Location location;
		Handle(Poly_Triangulation) triFace = BRep_Tool::Triangulation(aFace, location);
		if (triFace.IsNull())
			continue;
		Poly::ComputeNormals(triFace);
		int nbTri = triFace->NbTriangles();
		auto nodes = triFace->InternalNodes();
		auto uvNodes = triFace->InternalUVNodes();
		auto normals = triFace->InternalNormals();

		Standard_Real UMin = 0, UMax = 0, VMin = 0, VMax = 0;
		const gp_Trsf& aTrsf = location.Transformation();
		Standard_Boolean isMirrored = aTrsf.VectorialPart().Determinant() < 0;

		BRepTools::UVBounds(aFace, UMin, UMax, VMin, VMax);
		for (Standard_Integer aNodeIter = nodes.Lower(); aNodeIter <= nodes.Upper(); ++aNodeIter)
		{
			gp_Pnt vertex = nodes.Value(aNodeIter);
			gp_Pnt2d uv = uvNodes.Value(aNodeIter);
			gp_Vec3f normalf = normals.Value(aNodeIter);
			gp_Dir normal = gp_Dir(normalf.x(), normalf.y(), normalf.z());
			if ((aFace.Orientation() == TopAbs_REVERSED) ^ isMirrored)
			{
				normal.Reverse();
			}
			if (!location.IsIdentity())
			{
				vertex.Transform(aTrsf);
				normal.Transform(aTrsf);
			}

			triVertices->push_back(osg::Vec3(vertex.X(), vertex.Y(), vertex.Z()));
			triNormals->push_back(osg::Vec3(normal.X(), normal.Y(), normal.Z()));

			float x = uv.X(), y = uv.Y();
			triUvs->push_back(osg::Vec2((x - UMin), (y - VMin)));
		}

		int max = 0;
		// a mesh per occ shape, indexs increase in a shape
		const Poly_Array1OfTriangle& aTriangles = triFace->Triangles();
		for (int i = 1; i <= nbTri; i++)
		{
			Poly_Triangle aTriangle = triFace->Triangle(i);
			int index1 = 0;
			int index2 = 0;
			int index3 = 0;
			if ((aFace.Orientation() == TopAbs_REVERSED))
			{
				index1 = aTriangle.Value(1) - 1;
				index2 = aTriangle.Value(3) - 1;
				index3 = aTriangle.Value(2) - 1;
			}
			else
			{
				index1 = aTriangle.Value(1) - 1;
				index2 = aTriangle.Value(2) - 1;
				index3 = aTriangle.Value(3) - 1;
			}
			if (index1 > max)
			{
				max = index1;
			}
			if (index2 > max)
			{
				max = index2;
			}
			if (index3 > max)
			{
				max = index3;
			}

			triIndexs->push_back(index1 + newStart);
			triIndexs->push_back(index2 + newStart);
			triIndexs->push_back(index3 + newStart);
		}

		newStart += max + 1;
	}

	triGeom->setVertexArray(triVertices.get());
	triGeom->setNormalArray(triNormals);
	triGeom->setNormalBinding(osg::Geometry::BIND_PER_VERTEX);
	triGeom->setTexCoordArray(0, triUvs.get());
	triGeom->addPrimitiveSet(triIndexs.get());
	osg::ref_ptr<osg::Material>material = new osg::Material;
	osg::Vec4 color(1, 1, 1, 1.0);
	geode->getOrCreateStateSet()->setAttributeAndModes(material, osg::StateAttribute::ON);
	geode->addChild(triGeom.get());
	return geode.get();
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
	QTTEST w;

	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer;
	osg::ref_ptr<osg::Group> root = new osg::Group;
	
	const TopoDS_Shape box = BRepPrimAPI_MakeBox(10, 10, 10).Shape();
	osg::ref_ptr<osg::Node> node = BuildShape(box);
	root->addChild(node);

	viewer->setSceneData(root.get());
	viewer->setCameraManipulator(new osgGA::TrackballManipulator);

	osgQt::setViewer(viewer.get());
	osgQt::GLWidget* glw = new osgQt::GLWidget;
	osgQt::GraphicsWindowQt* graphicswin = new osgQt::GraphicsWindowQt(glw);
	graphicswin->setCursor(osgQt::GraphicsWindowQt::MouseCursor::HandCursor);
	viewer->getCamera()->setViewport(new osg::Viewport(0, 0, glw->width(), glw->height()));
	viewer->getCamera()->setGraphicsContext(graphicswin);
	viewer->setThreadingModel(osgViewer::ViewerBase::SingleThreaded);

	QGridLayout* grid = new QGridLayout;
	grid->addWidget(glw);
	grid->setSpacing(0);
	grid->setContentsMargins(0, 0, 0, 0);

	w.ui.listWidget->setLayout(grid);
	w.show();
    return a.exec();
}
