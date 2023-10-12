
import React from "react";
import { StyleSheet } from "react-native";
import { View } from "react-native-web";



const styles = StyleSheet.create({
    center: {
        alignItems: 'center',
    },
});

const Greeting = props => {
    return (
        <View style={[styles.center, { top: 50 }]}>
            <Text> Hello {props.name}! </Text>
        </View>
    )
}

export const LostsOffGreetings = () => {
    return (
        <view style={[styles.center, { top: 50 }]}>
            <Greeting name="a" />
            <Greeting name="b" />
            <Greeting name="c" />
        </view>

    )
}