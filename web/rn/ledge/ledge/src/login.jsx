

import React, { useEffect, useState } from "react";
import { Button, Dimensions, Pressable, StyleSheet } from "react-native";
import { View, Text, TextInput } from "react-native";

import { createStackNavigator } from '@react-navigation/stack';

import { NavigationContainer } from '@react-navigation/native'

function LoginPanel({ navigation }) {

    const [account, setAccount] = useState()
    const [password, setPassword] = useState()

    useEffect(() => {
        console.log(account)
        console.log(password)
    }, [account, password])



    const tryLogin = () => {
        console.log(account)
        console.log(password)
        if (account == undefined || password == undefined) {
            alert("Undefined acount/password")
            return;
        }
        if (account == "" || password == "") {
            alert("Empry acount/password")
            return;
        }
        if (account.length == 0 || password == 0) {
            alert("Empry acount/password")
            return;
        }

    }

    return (
        <View style={styles.page}>

            <View style={styles.panel}>
                <View style={styles.line}>
                    <Text >Account  : </Text>
                    <TextInput
                        onChangeText={text => setAccount(text)}
                        style={styles.input}
                    />
                </View>
                <View style={styles.line}>
                    <Text >Password : </Text>
                    <TextInput
                        onChangeText={text => setPassword(text)}
                        style={styles.input}
                    />
                </View>
                <View style={styles.lineButtons}>
                    <Pressable
                        style={styles.buttons}
                        onPressOut={tryLogin}
                    >
                        <Text>Login</Text>
                    </Pressable>
                    <Pressable
                        style={styles.buttons}
                        onPressOut={() => navigation.navigate("Register")}
                    >
                        <Text>Register</Text>
                    </Pressable>
                </View>

            </View>

        </View >
    )
}


function RegisterPanel() {

}

export default function Login() {

    const Stack = createStackNavigator();

    return (
        <NavigationContainer>
            <Stack.Navigator initialRouteName="Login"  >
                <Stack.Screen
                    name="Login"
                    component={LoginPanel}
                    options={{ headerShown: false }}
                />
                <Stack.Screen
                    name="Register"
                    component={RegisterPanel}
                />
            </Stack.Navigator>
        </NavigationContainer>
    );

    return AppNavigator;
}

const styles = StyleSheet.create({
    page: {
        flex: 1,
        justifyContent: 'center',
        alignContent: 'stretch',
        alignItems: 'center',
        backgroundColor: '#000',
        width: "100%",
        height: "100%"
    },
    panel: {
        justifyContent: 'center',
        alignContent: 'stretch',
        alignItems: 'center',
        width: "30%",
        height: '30%',
        backgroundColor: '#432',
        borderRadius: 13,
    },
    line: {
        // flex: 1,
        backgroundColor: '#434',
        flexDirection: 'row',
        justifyContent: 'content',
        alignItems: 'center',
        // width: "70%",
        // height: "40%",
        padding: 13,
    },
    lineButtons: {
        flexDirection: 'row',
        justifyContent: 'center',
    },
    buttons: {
        paddingVertical: "20%",
        marginTop: "20%",
        paddingHorizontal: "40%",
        marginHorizontal: "20%",
        backgroundColor: "#811",
        borderRadius: 20,
    },
    input: {
        backgroundColor: "#789",
        height: "150%",
        borderRadius: 5,
        borderWidth: 1,
    },

})