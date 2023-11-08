
import React, { useEffect, useState } from "react";
import { Button, Dimensions, Pressable, StyleSheet } from "react-native";
import { View } from "react-native";
import { Image } from "react-native";
import { Text } from "react-native";

import { AiOutlineLeft, AiOutlineRight } from "react-icons/ai"

const Arrow = (props) => {
	const Tag = props.bLeft ? AiOutlineLeft : AiOutlineRight;
	const Alt = props.bLeft ? "left arrow" : "right arrow";
	const padding = props.bLeft ? "paddingLeft" : "paddingRight";


	const getArrowSize = () => {
		let style = {
			width: props.windowSize.w / 20,
			height: props.windowSize.h / 3,
			[padding]: 30,
		};
		return style
	}


	return (
		<Tag
			alt={Alt}
			preserveAspectRatio="none"
			style={StyleSheet.flatten([getArrowSize(), styles.arrow])}
		// style={getArrowSize()}
		/>
	)
}

export const Greeting = (props) => {

	const GoBack = () => {
		props.setFirstIn(false)

		/**
		 * did not want to use the stack
		 * because the start pannel also sceen as the stack with '<-' button 
		 * in the top-left
		 */
		// props.route.params.setFirstIn(true)
		// props.navigation.navigate('App');
	}

	const [pageIndex, setPageIndex] = useState(0)
	const [windowSize, setWindowSize] = useState({ w: 300, h: 300 })

	const MainImages = [
		require("../assets/arch.png"),
		require("../assets/block.png"),
	];

	const MainTexts = [
		"Welcome to the leage verse!",
		"Got It!"
	]
	const NumPages = MainImages.length;

	useEffect(() => {
		const updateWindowDimensions = () => {
			const { width, height } = Dimensions.get('window');
			setWindowSize({ w: width, h: height });
		};
		Dimensions.addEventListener('change', updateWindowDimensions);
		updateWindowDimensions();
		return () => {
			Dimensions.removeEventListener('change', updateWindowDimensions);
		}
	}, []);

	const fowardPage = ok => {
		let op = pageIndex + (ok ? 1 : -1);// console.log(op)
		let newIndex = Math.min(Math.max(0, op), 1)
		setPageIndex(newIndex); // console.log(pageIndex);
	}




	return (
		<View style={styles.container}>
			<View style={[styles.content]}>

				<Pressable onPressOut={ev => fowardPage(false)}>
					<Arrow bLeft={true} windowSize={windowSize} />
				</Pressable>


				<View style={[styles.flagContainer]}>
					<Image
						alt="flag"
						style={[
							styles.flag,
							{
								width: windowSize.w / 1.3,
								height: windowSize.h / 1.4,
								padding: 1,
								borderRadius: 20
							},
						]}
						source={MainImages[pageIndex]}
					/>
				</View>

				<Pressable onPressOut={ev => fowardPage(true)}>
					<Arrow bLeft={false} windowSize={windowSize} />
				</Pressable>

			</View>

			<View style={styles.bottom}>
				{
					pageIndex === NumPages - 1 ? (
						<Pressable
							style={[
								styles.btn,
								{ width: windowSize.w / 3, height: windowSize.h / 20 }
							]}
							onPressOut={GoBack}
						>
							<Text>
								{MainTexts[pageIndex]}
							</Text>
						</Pressable>
					) : (
						<Text maxFontSizeMultiplier={0}>   {MainTexts[pageIndex]}! </Text>
					)
				}
			</View>

		</View >
	)
}



const styles = StyleSheet.create({
	container: {
		flex: 1,
		alignContent: 'center',
		justifyContent: 'center',
		alignItems: 'center',
		backgroundColor: "#245"
	},
	content: {
		flex: 1,
		flexDirection: "row",
		alignItems: 'center'

	},
	arrow: {
		color: "#fff",
	},
	flag: {

	},
	flagContainer: {
		alignItems: 'center',
		paddingHorizontal: 30
	},

	bottom: {
		alignItems: 'center',
		flexDirection: "row",
		fontFamily: 'Heiti',
		paddingBottom: 100
	},
	btn: {
		flex: 1,
		alignItems: "center",
		justifyContent: 'center',
		backgroundColor: "#587",
		borderRadius: 15,
	}
});
