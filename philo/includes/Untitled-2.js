var s = "this is the string"
const reverse = (str, start, end) => {
	let temp;

	while (start < end) {
		temp = str[start]
		str[start] = str[end]
		str[end] = temp
		start++
		end--
	}
}

const reverseWords = (str) => {
	str = str.split("")
	let start = 0

	for (let end = 0; end < str.length; end++) {
		if (str[end] == ' ')
		{
			reverse(str, start, end - 1)
			start = end + 1
		}
	}
	reverse(str, start, str.length - 1)
	reverse(str, 0, str.length - 1)
	return str.join("")
}

console.log(reverseWords(s))

function twoSum(nums, target) {
	const hashTable = {}

	for (let i = 0; i < nums.length; i++) {
		const complement = target - nums[i];
		if (hashTable.hasOwnProperty(complement))
		{
			return [hashTable[complement], i]
		}
		hashTable[nums[i]] = i
	}
	return null;
}

function twoSum (array, target) {
	hashTable = {}
	for (i = 0; i < array.length; i++) {
		let complement = target - array[i]
		if (hashTable.hasOwnProperty(complement))
		{
			return [hashTable[complement], i]
		}
		hashTable[array[i]] = i;
	}
	return null;
}