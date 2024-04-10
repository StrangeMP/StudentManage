import ChatGemini

model = ChatGemini.model()
essay_prompt = '''
示例数据：
{ "论文名": "中国经济发展与对外开放问题研究", "期刊或会议名": "经济研究", "发表日期": "2024-06-25", "级别": "第二级－C类", "加分": 0.2, "审核状态": "通过" }
{ "论文名": "The Impact of Quantum Computing on Machine Learning Algorithms", "期刊或会议名": "ACM Transactions on Quantum Computing", "发表日期": "2024-06-25", "级别": "第一级－A类", "加分": 0.3, "审核状态": "通过" }
{ "论文名": "Applications of Graph Theory in Social Network Analysis", "期刊或会议名": "Discrete Mathematics", "发表日期": "2024-03-23", "级别": "第一级－B类", "加分": 0.15, "审核状态": "通过" }
{ "论文名": "The Effects of Automation on the Labor Market", "期刊或会议名": "The American Economic Review", "发表日期": "2023-03-23", "级别": "第二级－A类", "加分": 0.1, "审核状态": "通过" }


每篇论文都有一个"加分"属性，该属性应在{0.01,0.02,0.05,0.1,0.2,0.4}之间取值；还有一个"审核状态"属性，该属性只能在["审核中","通过","否决"]中取值。
请根据给出的示例数据，生成100个论文数据，用json输出。每行一个项目，无需填充空格，稍后我会自行格式化。
注意日期应该在近四年内，不得晚于当前日期（2024年四月）
应该至少给出文学、法学、理学、工学、社会学、心理学、史学、医学、政治学、计算机科学等领域的科研论文
所生成的数据应尽可能不同。理学、工学、计算机科学应为英文论文和期刊，文科类应为中文。
'''

model.beginChat(essay_prompt, "./GetTestData/__Essay.json")

