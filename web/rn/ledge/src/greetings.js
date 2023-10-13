
import React, {useEffect, useState} from "react";
import {Dimensions, StyleSheet} from "react-native";
import {View} from "react-native";
import {Image} from "react-native";
import {Text} from "react-native";




export const Greeting = props => {

	const [pageIndex, setPageIndex] = useState(0)

	const [windowSize, setWindowSize] = useState({w: 300, h: 300})

	const MainImages = [
		require("../assets/arch.png"),
		require("../assets/block.png"),
	];

	const MainTexts = [
		"Page1",
		"Page2"
	]

	useEffect(() => {
		const updateWindowDimensions = () => {
			const {width, height} = Dimensions.get('window');
			setWindowSize({w: width, h: height});
		};

		Dimensions.addEventListener('change', updateWindowDimensions);

		// Get initial window dimensions
		updateWindowDimensions();

		return () => {
			Dimensions.removeEventListener('change', updateWindowDimensions);
		}
	}, []);


	return (
		<>
			<View style={styles.container}>
				<View style={[styles.upper, {top: 50}]}>
					<Image style={{width: windowSize.w / 2, height: windowSize.h / 2, padding: 10, borderRadius: 15}}
						source={MainImages[pageIndex]}
					/>
				</View>

				<View style={[styles.bottom, {top: 50}]}>
					<Text> Hello {MainTexts[pageIndex]}! </Text>
				</View>
			</View>
		</>
	)
}


const styles = StyleSheet.create({
	container: {
		marginLeft: 100,
		alignContent: 'center',
		justifyContent: 'center',
		alignItems: 'center'
	},
	upper: {
		alignItems: 'center',
		paddingBottom: 50
	},
	bottom: {
		alignItems: 'center',

	},
});
