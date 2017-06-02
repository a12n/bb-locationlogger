import bb.cascades 1.3
import QtQuick 1.0

Container {
    id: root
    background: Color.Black

    onVisibleChanged: {
        if (_gpxFile.isOpen()) {
            image.filterColor = Color.create("#ff0092cc")
        } else {
            image.filterColor.reset()
        }
        if (visible) {
            _geoLocation.error.connect(onError)
            _geoLocation.warning.connect(onWarning)
        } else {
            _geoLocation.error.disconnect(onError)
            _geoLocation.warning.disconnect(onWarning)
        }
    }

    function onError(err) {
        image.filterColor = Color.create("#ffff3333")
    }

    function onWarning(warn) {
        image.filterColor = Color.create("#ffdcd427")
    }

    attachedObjects: [
        Timer {
            id: timer
            interval: 30000
            repeat: true
            triggeredOnStart: true
            running: root.visible
            onTriggered: {
                latLabel.text = _geoLocation.latitude.toFixed(3) + "°"
                lonLabel.text = _geoLocation.longitude.toFixed(3) + "°"
            }
        }
    ]

    layout: DockLayout {
    }

    Container {
        verticalAlignment: VerticalAlignment.Center
        horizontalAlignment: HorizontalAlignment.Center
        ImageView {
            id: image
            imageSource: "asset:///images/ic_map.png"
            verticalAlignment: VerticalAlignment.Center
            horizontalAlignment: HorizontalAlignment.Center
            scalingMethod: ScalingMethod.AspectFill
        }
        Label {
            id: latLabel
            text: "N/A°"
            textStyle.fontSize: FontSize.Large
            horizontalAlignment: HorizontalAlignment.Center
        }
        Label {
            id: lonLabel
            text: "N/A°"
            textStyle.fontSize: FontSize.Large
            horizontalAlignment: HorizontalAlignment.Center
        }
    }
}
