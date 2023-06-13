import { StyleSheet, Image } from "react-native";

export default function ImageViewer({ placeHolderImage, selectedImage }) {
  const image =
    selectedImage !== null ? { uri: selectedImage } : placeHolderImage;

  return <Image source={image} style={styles.image} />;
}

const styles = StyleSheet.create({
  image: {
    width: 200,
    height: 300,
    borderRadius: 18,
  },
});
