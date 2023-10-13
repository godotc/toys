import {useState} from "react";
import {Pressable, StyleSheet, TextInput} from "react-native";
import {Text} from "react-native";
import {FetchResult} from "react-native";

const print = msg => console.log(msg)


export const RecoredPage = () => {
	const [content, setContent] = useState();
	const default_color = "purple";
	const [btnColor, setBtnColor] = useState(default_color);

	return (
		<div style={styles.page}>

			<TextInput
				style={styles.RecorederInput}
				onChangeText={(msg) => {print(msg.length == 0 ? "Handle text change failed" : msg); setContent(msg);}}
			/>

			<br />

			<Pressable
				style={[styles.SubmitButton, {backgroundColor: btnColor}]}
				onPress={() => print(content)}
				onHoverIn={() => {setBtnColor("#333");}}
				onHoverOut={() => {setBtnColor(default_color);}}

				children={<Text children="Submit" />}
			/>
		</div>
	);
};

const styles = StyleSheet.create({
	page: {
		alignContent: "center",
		justifyContent: "center",
		alignItems: "center",
		alignSelf: "center",
	},
	RecorederInput: {
		backgroundColor: "#445",
		width: 120,
		borderRadius: 3,
	},
	SubmitButton: {
		width: 70,
		height: 20,
		borderRadius: 15,
		backgroundColor: "#429",
		justifyContent: "center",
		alignItems: "center",
	},
});

