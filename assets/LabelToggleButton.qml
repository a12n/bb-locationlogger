import bb.cascades 1.3

Container {
    property alias text: label.text
    property alias checked: toggle.checked

    layout: StackLayout {
        orientation: LayoutOrientation.LeftToRight
    }

    Label {
        id: label
        layoutProperties: StackLayoutProperties {
            spaceQuota: 1.0
        }
        textStyle {
            base: SystemDefaults.TextStyles.BodyText
        }
    }

    ToggleButton {
        id: toggle
        layoutProperties: StackLayoutProperties {
            spaceQuota: 1.0
        }
    }
}
