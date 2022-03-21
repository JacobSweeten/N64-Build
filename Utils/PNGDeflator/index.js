const sharp = require('sharp');
const fs = require('fs');

function swapEndianness16(val)
{
	var outVal = (val >> 8) & 0x00FF;
	outVal += (val << 8) & 0xFF00;
	return outVal;
}

if(process.argv.length != 4)
{
	console.log("ERROR: Please pecify input and output file.");
	process.exit(1);
}

sharp(process.argv[2]).raw().toBuffer().then(async (data) => {
	var arr = Uint8Array.from(data);

	var outArr = new Uint16Array((arr.length / 3) + 2);
	var outArrIdx = 2;

	var metadata = await sharp(process.argv[2]).metadata();
	outArr[0] = swapEndianness16(metadata.height);
	outArr[1] = swapEndianness16(metadata.width);

	for(var i = 0; i < arr.length; i+=3)
	{
		var r = arr[i];
		var g = arr[i + 1];
		var b = arr[i + 2];

		var newR = Math.round((r / 255) * 31);
		var newG = Math.round((g / 255) * 31);
		var newB = Math.round((b / 255) * 31);

		var outColor = newR << 11;
		outColor += newG << 6;
		outColor += newB << 1;
		outColor += 1;

		outArr[outArrIdx] = swapEndianness16(outColor);
		outArrIdx++;
	}

	fs.writeFileSync(process.argv[3], Buffer.from(outArr.buffer));
});