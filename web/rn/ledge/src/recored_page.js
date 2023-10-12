import { useState } from "react";
import { Text, TextInput } from "react-native";
import { View } from "react-native";
import { StyleSheet } from "react-native";


export const RecoredPage = () => {

    const print = msg => console.log(msg)
    const [content, setConent] = useState();

    return (

        <div>
            <Text>
                Here to write what you have dereamed about last night/today:
            </Text>
            <br />
            <div style={styles.inputContainer} >

                <TextInput style={styles.input}
                    onChange={(e) => { setConent(e.value); print("content is changed: ", e.value) }} >
                </TextInput >

            </div >
        </div>

    );
}


const styles = StyleSheet.create({
    inputContainer: {
        padding: 3,
        backgroundColor: "#f456"
    },
    input: {
        backgroundColor: "#f444"
    }
})