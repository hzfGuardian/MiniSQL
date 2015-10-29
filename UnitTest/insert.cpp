printf("Call API_Insert\n");
cout << "Table name: " << tuple.table_name << endl;
cout << "Attr count: " << tuple.attr_count << endl;
cout << "===========" << endl;
for (int i = 0; i < tuple.attr_count; ++i)
{
    cout << "Attr " << i << ": " << endl;
    cout << "Attribute name: " << tuple.attrs[i].attr_name << endl;
    cout << "Attribute type: " << tuple.attrs[i].attr_type << endl;
    cout << "Attribute key type: " << tuple.attrs[i].attr_key_type << endl;
    cout << "Attribute length: " << tuple.attrs[i].attr_len << endl;
    cout << "Attribute id: " << tuple.attrs[i].attr_id << endl;
    cout << "---" << endl << tuple.attr_values[i] << endl;
}